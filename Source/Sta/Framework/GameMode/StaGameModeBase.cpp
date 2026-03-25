// Sta Project - Multiplay RTS with GAS


#include "StaGameModeBase.h"

#include "AbilitySystemComponent.h"
#include "Area/AreaBase.h"
#include "Character/AIPawn.h"
#include "Controller/StaPlayerController.h"
#include "Framework/GameState/StaGameState.h"
#include "Character/CommandPawn.h"
#include "Controller/StaAIController.h"
#include "DataAsset/AIStyleData.h"
#include "DataAsset/CardData.h"
#include "Framework/PlayerState/StaPlayerState.h"
#include "Helper/StaHelper.h"
#include "Kismet/GameplayStatics.h"
#include "UI/StaHUD.h"

AStaGameModeBase::AStaGameModeBase()
{
	DefaultPawnClass = ACommandPawn::StaticClass();
	HUDClass = AStaHUD::StaticClass();
	PlayerControllerClass = AStaPlayerController::StaticClass();
	GameStateClass = AStaGameState::StaticClass();
	PlayerStateClass = AStaPlayerState::StaticClass();
	
}

void AStaGameModeBase::BeginPlay()
{
	Super::BeginPlay();

}

void AStaGameModeBase::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	if (IGenericTeamAgentInterface* TeamAgentInterface = Cast<IGenericTeamAgentInterface>(NewPlayer->PlayerState))
	{
		check(CurrentTeamNum < 255);
		
		FGenericTeamId NewTeamId(CurrentTeamNum);
		TeamAgentInterface->SetGenericTeamId(NewTeamId);

		CurrentTeamNum++;

		if (CurrentTeamNum == PlayerNum)
		{
			FTimerHandle StartTimer;
			GetWorldTimerManager().SetTimer(StartTimer, this, &ThisClass::StartGameSettings, 0.1f, false);
		}
	}
}

void AStaGameModeBase::StartGameSettings()
{
	SpawnAIPawns();
	InitAreaState();
	InItPlayerState();
}

void AStaGameModeBase::SpawnAIPawns()
{
	if (!AIPawnClass || !AIControllerClass || !GetWorld()) return;

	for (int i = 0; i < AINum; i++)
	{
		AStaAIController* SpawnedAIController = GetWorld()->SpawnActor<AStaAIController>(AIControllerClass);
		if (!SpawnedAIController || !SpawnedAIController->GetDeckData()) continue;

		IGenericTeamAgentInterface* TeamAgentInterface = Cast<IGenericTeamAgentInterface>(SpawnedAIController->GetPlayerState<APlayerState>());
		if (!TeamAgentInterface) continue;
		
		AAIPawn* SpawnedAI = GetWorld()->SpawnActor<AAIPawn>(AIPawnClass);
		if(!SpawnedAI) continue;

		FGenericTeamId NewTeamId(CurrentTeamNum);
		TeamAgentInterface->SetGenericTeamId(NewTeamId);

		CurrentTeamNum++;

		SpawnedAIController->Possess(SpawnedAI);
		InitDeckState(SpawnedAIController->GetDeckData()->DeckList, SpawnedAIController);
		
	}
}

void AStaGameModeBase::InitAreaState()
{
	if (!GetGameState<AGameStateBase>()) return;
	
	//TODO: Temp Area Assign
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AAreaBase::StaticClass(), FoundActors);

	int j = 0;
	for (int i = 0; i < GetGameState<AGameStateBase>()->PlayerArray.Num(); i++)
	{
		for (; j < FoundActors.Num(); j++)
		{
			if (AAreaBase* Area = Cast<AAreaBase>(FoundActors[j]))
			{
				if (Area->GetGenericTeamId() == FGenericTeamId::NoTeam)
				{
					Area->SetGenericTeamId(i);
					break;
				}
			}
		}
	}
}

void AStaGameModeBase::InItPlayerState()
{
	check(InitPlayerStateEffectClass);

	for (APlayerState* PlayerState : GetGameState<AGameStateBase>()->PlayerArray)
	{
		IAbilitySystemInterface* PlayerAbilityInterface = Cast<IAbilitySystemInterface>(PlayerState);
		if (!PlayerAbilityInterface) continue;;

		UAbilitySystemComponent* AbilitySystemComponent = PlayerAbilityInterface->GetAbilitySystemComponent();
		if (!AbilitySystemComponent) continue;

		AbilitySystemComponent->ApplyGameplayEffectToSelf(
			InitPlayerStateEffectClass->GetDefaultObject<UGameplayEffect>(), 1.0f, AbilitySystemComponent->MakeEffectContext());
	}

}

void AStaGameModeBase::InitDeckState(const TArray<FCardInfo>& DeckList, AController* Controller)
{
	if (DeckStates.Contains(Controller)) return;

	FDeckState DeckState;
	for (FCardInfo CardInfo : DeckList)
	{
		for (int i = 0; i < CardInfo.CardNum; i++)
		{
			DeckState.DeckCards.Add(CardInfo.CardData);
		}
	}
	
	ShuffleDeck(DeckState.DeckCards);
	DeckStates.Emplace(Controller, DeckState);

	for (int i = 0; i < InitCardNum; i++)
	{
		DrawCard(Controller);
	}
}

bool AStaGameModeBase::CanDrawCard(const AController* Controller)
{
	if (!Controller) return false;

	FDeckState* DeckState = DeckStates.Find(Controller);
	if (!DeckState) return false;

	return DeckState->HandCards.Num() < MaxCardNum;
}

bool AStaGameModeBase::CardInHand(const AController* Controller, const UCardData* CardData)
{
	if (!Controller) return false;
	
	FDeckState* DeckState = DeckStates.Find(Controller);
	if (!DeckState) return false;

	return DeckState->HandCards.Contains(CardData);
}

const TArray<const UCardData*>& AStaGameModeBase::GetHandCards(const AController* Controller) const
{
	static const TArray<TObjectPtr<const UCardData>> Empty;
	
	if (const FDeckState* State = DeckStates.Find(Controller))
	{
		return State->HandCards;
	}
	else
	{
		return Empty;
	}
}

void AStaGameModeBase::DrawCard(AController* Controller)
{
	if (!Controller || !CanDrawCard(Controller)) return;

	FDeckState* DeckState = DeckStates.Find(Controller);
	if (!DeckState) return;

	if (DeckState->DeckCards.Num() == 0)
	{
		DeckState->DeckCards.Append(DeckState->DiscardCards);
		DeckState->DiscardCards.Empty();

		if (DeckState->DeckCards.Num() == 0) return;
		
		ShuffleDeck(DeckState->DeckCards);
	}

	const UCardData* DrawCard = DeckState->DeckCards.Pop();
	if (!DrawCard) return;
	
	DeckState->HandCards.Add(DrawCard);

	if (AStaPlayerController* StaPC = Cast<AStaPlayerController>(Controller))
	{
		StaPC->ClientDrawCard(DrawCard);
	}
	else if (AStaAIController* StaAIC = Cast<AStaAIController>(Controller))
	{
		//StaAIC->DrawCard()
	}
	else
	{
		check(false);
	}
	
}

void AStaGameModeBase::DiscardCard(AController* Controller, const UCardData* CardData)
{
	if (!Controller || !CardData) return;
	if (!CardInHand(Controller, CardData)) return;

	FDeckState* DeckState = DeckStates.Find(Controller);
	if (!DeckState) return;
	
	DeckState->DiscardCards.Add(CardData);
	DeckState->HandCards.RemoveSingle(CardData);
	
	if (AStaPlayerController* StaPC = Cast<AStaPlayerController>(Controller))
	{
		StaPC->ClientDiscardCard(CardData);
	}
	else if (AStaAIController* StaAIC = Cast<AStaAIController>(Controller))
	{
		//StaAIC->DiscardCard()
	}
	else
	{
		check(false);
	}
	
}

void AStaGameModeBase::ShuffleDeck(TArray<TObjectPtr<const UCardData>>& Cards)
{
	if (Cards.Num() <= 1) return;

	StaDebug::Print("ShuffleDeck");
	for (int i = Cards.Num() - 1; i > 0; i--)
	{
		int RandIndex = FMath::RandRange(0, i);
		Cards.Swap(i, RandIndex);
	}
	
}
