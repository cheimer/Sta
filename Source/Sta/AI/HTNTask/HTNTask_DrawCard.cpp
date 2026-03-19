// Sta Project - Multiplay RTS with GAS


#include "HTNTask_DrawCard.h"

#include "Abilities/GameplayAbilityTypes.h"
#include "Controller/StaAIController.h"
#include "GameplayTag/StaTags.h"

bool UHTNTask_DrawCard::CheckPrecondition(const FHTNWorldState& WS) const
{
	if (!WS.bCanDraw) return false;
	if (WS.HandCards.Num() >= WS.MaxHandSize) return false;
	if (WS.CurrentCost < DrawCost) return false;

	return true;
}

void UHTNTask_DrawCard::SimulateEffect(FHTNWorldState& WS) const
{
	WS.CurrentCost -= DrawCost;
	WS.bCanDraw = false;
}

EHTNTaskStatus UHTNTask_DrawCard::Execute(AStaAIController* Owner, const FHTNWorldState& WS) const
{
	FGameplayEventData EventData;
	EventData.Target = Owner->GetPawn();

	Owner->RequestAction(StaTags::Event::Card::Draw, &EventData);
	
	return EHTNTaskStatus::Success;
}
