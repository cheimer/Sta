// Sta Project - Multiplay RTS with GAS


#include "StaGameModeBase.h"

#include "Area/AreaBase.h"
#include "Controller/StaPlayerController.h"
#include "Framework/GameState/StaGameState.h"
#include "Player/CommandPawn.h"
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

	if (bIsSinglePlay)
	{
		//PlayerNum = 1;
	}

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
			InitAreaState();
		}
	}
}

void AStaGameModeBase::InitDeckState(const TArray<FCardInfo>& DeckList, APlayerController* PC)
{
	if (PlayerDeckState.Contains(PC)) return;

	FPlayerDeckState DeckState;
	for (FCardInfo CardInfo : DeckList)
	{
		for (int i = 0; i < CardInfo.CardNum; i++)
		{
			DeckState.DeckCards.Add(CardInfo.CardData);
		}
	}
	
	ShuffleDeck(DeckState.DeckCards);
	PlayerDeckState.Emplace(PC, DeckState);

	for (int i = 0; i < InitCardNum; i++)
	{
		DrawCard(PC);
	}
}

void AStaGameModeBase::InitAreaState()
{
	//TODO: Temp Area Assign
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AAreaBase::StaticClass(), FoundActors);

	int IterIndex = 0;
	for (FConstPlayerControllerIterator PCIter = GetWorld()->GetPlayerControllerIterator(); PCIter; ++PCIter)
	{
		for (AActor* FoundActor : FoundActors)
		{
			if (AAreaBase* Area = Cast<AAreaBase>(FoundActor))
			{
				if (Area->GetGenericTeamId() == FGenericTeamId::NoTeam)
				{
					Area->SetGenericTeamId(IterIndex);
					IterIndex++;
					break;
				}
			}
		}
	}
}

bool AStaGameModeBase::CanDrawCard(APlayerController* PC)
{
	if (!PC) return false;

	FPlayerDeckState* DeckState = PlayerDeckState.Find(PC);
	if (!DeckState) return false;

	return DeckState->HandCards.Num() < MaxCardNum;
}

bool AStaGameModeBase::CardInHand(APlayerController* PC, const UCardData* CardData)
{
	if (!PC) return false;
	
	FPlayerDeckState* DeckState = PlayerDeckState.Find(PC);
	if (!DeckState) return false;

	return DeckState->HandCards.Contains(CardData);
}

void AStaGameModeBase::DrawCard(APlayerController* PC)
{
	AStaPlayerController* StaPC = Cast<AStaPlayerController>(PC);
	if (!StaPC) return;

	FPlayerDeckState* DeckState = PlayerDeckState.Find(PC);
	if (!DeckState) return;

	if (!CanDrawCard(PC)) return;

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

void AStaGameModeBase::DiscardCard(APlayerController* PC, const UCardData* CardData)
{
	AStaPlayerController* StaPC = Cast<AStaPlayerController>(PC);
	if (!StaPC) return;
	
	FPlayerDeckState* DeckState = PlayerDeckState.Find(PC);
	if (!DeckState) return;
	
	if (!CardInHand(PC, CardData)) return;

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
