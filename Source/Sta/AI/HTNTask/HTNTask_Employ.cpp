// Sta Project - Multiplay RTS with GAS


#include "HTNTask_Employ.h"

#include "Abilities/GameplayAbilityTypes.h"
#include "Area/AreaBase.h"
#include "Controller/StaAIController.h"
#include "GameplayTag/StaTags.h"

bool UHTNTask_Employ::CheckPrecondition(const FHTNWorldState& WS) const
{
	if (TargetAreaIndex == INDEX_NONE) return false;
	if (WS.Areas[TargetAreaIndex].OwningTeam != WS.MyTeamId) return false;
	if (WS.CurrentCost < EmployCost) return false;
	
	return true;
}

void UHTNTask_Employ::SimulateEffect(FHTNWorldState& WS) const
{
	WS.Areas[TargetAreaIndex].UnitNum += EmployNum;
	WS.CurrentCost -= EmployCost;
}

EHTNTaskStatus UHTNTask_Employ::Execute(AStaAIController* Owner, const FHTNWorldState& WS) const
{
	AAreaBase* TargetArea = WS.Areas[TargetAreaIndex].AreaActor.Get();
	if (!TargetArea) return EHTNTaskStatus::Failed;

	FGameplayEventData EventData;
	FGameplayAbilityTargetData_ActorArray* AreaArray = new FGameplayAbilityTargetData_ActorArray();
	AreaArray->TargetActorArray.Add(TargetArea);
	EventData.TargetData.Add(AreaArray);
	EventData.EventMagnitude = EmployNum;

	Owner->RequestAction(StaTags::Event::Area::Employ, &EventData);
	
	return EHTNTaskStatus::Success;
}
