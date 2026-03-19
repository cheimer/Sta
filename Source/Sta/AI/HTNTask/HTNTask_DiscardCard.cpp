// Sta Project - Multiplay RTS with GAS


#include "HTNTask_DiscardCard.h"

#include "Abilities/GameplayAbilityTypes.h"
#include "Controller/StaAIController.h"
#include "DataAsset/CardData.h"

bool UHTNTask_DiscardCard::CheckPrecondition(const FHTNWorldState& WS) const
{
	if (CardIndex < 0 || CardIndex >= WS.HandCards.Num()) return false;

	return true;
}

void UHTNTask_DiscardCard::SimulateEffect(FHTNWorldState& WS) const
{
	WS.HandCards.RemoveAt(CardIndex);
}

EHTNTaskStatus UHTNTask_DiscardCard::Execute(AStaAIController* Owner, const FHTNWorldState& WS) const
{
	const UCardData* Card = WS.HandCards[CardIndex].CardData.Get();
	if (!Card) return EHTNTaskStatus::Failed;

	FGameplayEventData EventData;
	EventData.Instigator = Owner->GetPawn();
	EventData.OptionalObject = Card;

	Owner->RequestAction(StaTags::Event::Card::Discard, &EventData);
	
	return EHTNTaskStatus::Success;
}
