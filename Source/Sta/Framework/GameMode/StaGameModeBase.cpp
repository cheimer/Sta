// Sta Project - Multiplay RTS with GAS


#include "StaGameModeBase.h"

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
			GetWorldTimerManager().SetTimerForNextTick(this, &ThisClass::StartGameSettings);
		}
	}
}

void AStaGameModeBase::StartGameSettings()
{
	SpawnAIPawns();
	InitAreaState();
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

void AStaGameModeBase::DrawCard(AController* Controller)
{
	AStaPlayerController* StaPC = Cast<AStaPlayerController>(Controller);
	if (!StaPC) return;

	FDeckState* DeckState = DeckStates.Find(Controller);
	if (!DeckState) return;

	if (!CanDrawCard(Controller)) return;

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

	StaPC->ClientDrawCard(DrawCard);
	
}

void AStaGameModeBase::DiscardCard(AController* Controller, const UCardData* CardData)
{
	AStaPlayerController* StaPC = Cast<AStaPlayerController>(Controller);
	if (!StaPC) return;
	
	FDeckState* DeckState = DeckStates.Find(Controller);
	if (!DeckState) return;
	
	if (!CardInHand(Controller, CardData)) return;

	DeckState->DiscardCards.Add(CardData);
	DeckState->HandCards.RemoveSingle(CardData);
	
	StaPC->ClientDiscardCard(CardData);
	
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
