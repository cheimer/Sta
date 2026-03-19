// Sta Project - Multiplay RTS with GAS


#include "BTDeco_NeedRePlan.h"

#include "Component/HTNComponent.h"
#include "Controller/StaAIController.h"

UBTDeco_NeedRePlan::UBTDeco_NeedRePlan()
{
	NodeName = "Is Need Re Plan?";
}

bool UBTDeco_NeedRePlan::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	AStaAIController* OwnerController = Cast<AStaAIController>(OwnerComp.GetAIOwner());
	if (!OwnerController) return true;

	UHTNComponent* HTNComp = OwnerController->FindComponentByClass<UHTNComponent>();
	if (!HTNComp) return true;

	const FHTNWorldState& Cur = HTNComp->GetCurrentWorldState();
	const FHTNWorldState& Plan = HTNComp->GetPlannedWorldState();
	if (Cur.Areas.Num() != Plan.Areas.Num()) return true;
	
	if (!HTNComp->HasValidPlan())
	{
		return true;
	}
	
	for (int32 i = 0; i < Cur.Areas.Num(); i++)
	{
		if (Cur.Areas[i].OwningTeam != Plan.Areas[i].OwningTeam)
		{
			return true;
		}
	}

	float CurTotal = 0.f, PlanTotal = 0.f;
	for (int32 i = 0; i < Cur.Areas.Num(); i++)
	{
		if (Cur.Areas[i].OwningTeam == Cur.MyTeamId)
		{
			CurTotal += Cur.Areas[i].UnitNum;
		}
			
		if (Plan.Areas[i].OwningTeam == Plan.MyTeamId)
		{
			PlanTotal += Plan.Areas[i].UnitNum;
		}
	}

	if (PlanTotal > 0.f && FMath::Abs(CurTotal - PlanTotal) / PlanTotal > UnitChangeThreshold)
	{
		return true;
	}

	return false;
}
