// Sta Project - Multiplay RTS with GAS


#include "HTNTask_Move.h"

#include "Abilities/GameplayAbilityTargetTypes.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "Area/AreaBase.h"
#include "Controller/StaAIController.h"
#include "GameplayTag/StaTags.h"

bool UHTNTask_Move::CheckPrecondition(const FHTNWorldState& WS) const
{
	if (SourceAreaIndex == INDEX_NONE || TargetAreaIndex == INDEX_NONE)
		return false;

	const FHTNAreaState& Src = WS.Areas[SourceAreaIndex];
	const FHTNAreaState& Dest = WS.Areas[TargetAreaIndex];

	if (Src.OwningTeam != WS.MyTeamId || Dest.OwningTeam != WS.MyTeamId) return false;
	if (Src.UnitNum < UnitCount) return false;
	if (!Src.ConnectedAreaIndices.Contains(TargetAreaIndex)) return false;

	return true;
}

void UHTNTask_Move::SimulateEffect(FHTNWorldState& WS) const
{
	WS.Areas[SourceAreaIndex].UnitNum -= UnitCount;
	WS.Areas[TargetAreaIndex].UnitNum += UnitCount;
}

EHTNTaskStatus UHTNTask_Move::Execute(AStaAIController* Owner, const FHTNWorldState& WS) const
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

	Owner->RequestAction(StaTags::Event::Area::Move, &EventData);
	
	return EHTNTaskStatus::Success;
}
