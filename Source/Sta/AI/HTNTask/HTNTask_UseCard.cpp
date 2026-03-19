// Sta Project - Multiplay RTS with GAS


#include "HTNTask_UseCard.h"

#include "Abilities/GameplayAbilityTypes.h"
#include "Area/AreaBase.h"
#include "Controller/StaAIController.h"
#include "DataAsset/CardData.h"

bool UHTNTask_UseCard::CheckPrecondition(const FHTNWorldState& WS) const
{
	if (CardIndex < 0 || CardIndex >= WS.HandCards.Num()) return false;
	if (TargetAreaIndex == INDEX_NONE) return false;
	if (WS.CurrentCost < WS.HandCards[CardIndex].Cost) return false;
	
	return true;
}

void UHTNTask_UseCard::SimulateEffect(FHTNWorldState& WS) const
{
	if (TargetAreaIndex == INDEX_NONE) return;

	const UCardData* Card = WS.HandCards[CardIndex].CardData.Get();
	if (!Card) return;

	WS.CurrentCost -= Card->Cost;
	for (const FCardModifier& CardModifier : Card->CardModifiers)
	{
		FHTNAreaState& Area = WS.Areas[TargetAreaIndex];

		if (CardModifier.ValueTag == StaTags::SetByCaller::UnitNum)
		{
			Area.UnitNum = FMath::Max(0.f, Area.UnitNum + CardModifier.Value);
		}
		else if (CardModifier.ValueTag == StaTags::SetByCaller::Defense)
		{
			Area.Defense = FMath::Max(0.f, Area.Defense + CardModifier.Value);
		}
		else if (CardModifier.ValueTag == StaTags::SetByCaller::BluffUnitNum)
		{
			Area.BluffUnitAdd += CardModifier.Value;
		}
		else if (CardModifier.ValueTag == StaTags::SetByCaller::BluffDefense)
		{
			Area.BluffDefenseAdd += CardModifier.Value;
		}
		else
		{
			checkf(false, TEXT("%s : Undefined SetByCaller"), *GetName());
		}
	}
	
	WS.HandCards.RemoveAt(CardIndex);
}

EHTNTaskStatus UHTNTask_UseCard::Execute(AStaAIController* Owner, const FHTNWorldState& WS) const
{
	const UCardData* Card = WS.HandCards[CardIndex].CardData.Get();
	AAreaBase* TargetArea = WS.Areas[TargetAreaIndex].AreaActor.Get();
	if (!Card || !TargetArea) return EHTNTaskStatus::Failed;

	FGameplayEventData EventData;
	EventData.Instigator = Owner->GetPawn();
	EventData.OptionalObject = Card;
	EventData.Target = TargetArea;

	Owner->RequestAction(StaTags::Event::Card::Use, &EventData);
	
	return EHTNTaskStatus::Success;
}
