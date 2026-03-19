// Sta Project - Multiplay RTS with GAS


#include "HTNTask_Scan.h"

#include "Abilities/GameplayAbilityTypes.h"
#include "Area/AreaBase.h"
#include "Component/HTNComponent.h"
#include "Controller/StaAIController.h"
#include "GameplayTag/StaTags.h"

bool UHTNTask_Scan::CheckPrecondition(const FHTNWorldState& WS) const
{
	if (TargetAreaIndex == INDEX_NONE) return false;
	
	if (WS.Areas[TargetAreaIndex].OwningTeam == WS.MyTeamId) return false;
	if (WS.CurrentCost < ScanCost) return false;
	
	return true;
}

void UHTNTask_Scan::SimulateEffect(FHTNWorldState& WS) const
{
    WS.Areas[TargetAreaIndex].LastScanTime = WS.CurrentTime;
    WS.CurrentCost -= ScanCost;
}

EHTNTaskStatus UHTNTask_Scan::Execute(AStaAIController* Owner, const FHTNWorldState& WS) const
{
	AAreaBase* TargetArea = WS.Areas[TargetAreaIndex].AreaActor.Get();
	if (!TargetArea) return EHTNTaskStatus::Failed;

	FGameplayEventData EventData;
	FGameplayAbilityTargetData_ActorArray* AreaArray = new FGameplayAbilityTargetData_ActorArray();
	AreaArray->TargetActorArray.Add(TargetArea);
	EventData.TargetData.Add(AreaArray);

	Owner->RequestAction(StaTags::Event::Area::Scan, &EventData);

	if (UHTNComponent* HTNComp = Owner->GetComponentByClass<UHTNComponent>())
	{
		HTNComp->RecordScan(TargetArea, WS.CurrentTime);
	}
	
	return EHTNTaskStatus::Success;
}
