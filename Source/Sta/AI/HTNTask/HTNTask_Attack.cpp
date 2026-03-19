// Sta Project - Multiplay RTS with GAS


#include "HTNTask_Attack.h"

#include "Abilities/GameplayAbilityTypes.h"
#include "Area/AreaBase.h"
#include "Component/HTNComponent.h"
#include "Controller/StaAIController.h"
#include "GameplayTag/StaTags.h"
#include "Helper/StaHelper.h"

bool UHTNTask_Attack::CheckPrecondition(const FHTNWorldState& WS) const
{
	if (SourceAreaIndex == INDEX_NONE || TargetAreaIndex == INDEX_NONE) return false;

	const FHTNAreaState& Src = WS.Areas[SourceAreaIndex];
	const FHTNAreaState& Dest = WS.Areas[TargetAreaIndex];

	if (Src.OwningTeam != WS.MyTeamId || Dest.OwningTeam == WS.MyTeamId) return false;
	if (Src.UnitNum < UnitCount) return false;
	if (!Src.ConnectedAreaIndices.Contains(TargetAreaIndex)) return false;

	return true;
}

void UHTNTask_Attack::SimulateEffect(FHTNWorldState& WS) const
{
	WS.Areas[SourceAreaIndex].UnitNum -= UnitCount;

	float EnemyDefenseValue = UAreaCalc::CalcGetDefenseValue( WS.Areas[TargetAreaIndex].GetAreaUnitNum(WS.MyTeamId), WS.Areas[TargetAreaIndex].GetAreaDefense(WS.MyTeamId));
	
	if (UnitCount > EnemyDefenseValue)
	{
		WS.Areas[TargetAreaIndex].OwningTeam = WS.MyTeamId;
		WS.Areas[TargetAreaIndex].UnitNum = UnitCount - EnemyDefenseValue;
		WS.Areas[TargetAreaIndex].Defense = 1.5f;
	}
	else
	{
		float RemainDefenseValue = EnemyDefenseValue - UnitCount;
		WS.Areas[TargetAreaIndex].UnitNum = UAreaCalc::CalcDefenseValueToUnitNum(RemainDefenseValue, WS.Areas[TargetAreaIndex].GetAreaDefense(WS.MyTeamId));
	}
}

EHTNTaskStatus UHTNTask_Attack::Execute(AStaAIController* Owner, const FHTNWorldState& WS) const
{
	AAreaBase* SrcArea = WS.Areas[SourceAreaIndex].AreaActor.Get();
	AAreaBase* DestArea = WS.Areas[TargetAreaIndex].AreaActor.Get();
	if (!SrcArea || !DestArea) return EHTNTaskStatus::Failed;
	
	FGameplayEventData EventData;
	FGameplayAbilityTargetData_ActorArray* AreaArray = new FGameplayAbilityTargetData_ActorArray();
	AreaArray->TargetActorArray.Add(SrcArea);
	AreaArray->TargetActorArray.Add(DestArea);
	EventData.TargetData.Add(AreaArray);
	EventData.EventMagnitude = UnitCount;

	Owner->RequestAction(StaTags::Event::Area::Attack, &EventData);
	
	return EHTNTaskStatus::Success;
}
