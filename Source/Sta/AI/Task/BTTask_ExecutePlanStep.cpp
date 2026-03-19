// Sta Project - Multiplay RTS with GAS


#include "BTTask_ExecutePlanStep.h"

#include "Component/HTNComponent.h"
#include "Component/HTNTypes.h"
#include "Controller/StaAIController.h"
#include "AI/HTNTask/HTNTaskBase.h"

UBTTask_ExecutePlanStep::UBTTask_ExecutePlanStep()
{
	NodeName = "Execute Plan Piecemeal";
}

EBTNodeResult::Type UBTTask_ExecutePlanStep::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	AStaAIController* OwnerController = Cast<AStaAIController>(OwnerComp.GetAIOwner());
	if (!OwnerController) return EBTNodeResult::Failed;

	UHTNComponent* HTNComp = OwnerController->FindComponentByClass<UHTNComponent>();
	if (!HTNComp || !HTNComp->HasValidPlan()) return EBTNodeResult::Failed;

	UHTNTaskBase* CurrentTask = HTNComp->GetCurrentTask();
	if (!CurrentTask)
	{
		HTNComp->CompletePlan();
		return EBTNodeResult::Succeeded;
	}

	if (!CurrentTask->CheckPrecondition(HTNComp->GetCurrentWorldState()))
	{
		HTNComp->InvalidatePlan();
		return EBTNodeResult::Failed;
	}

	EHTNTaskStatus Status = CurrentTask->Execute(OwnerController, HTNComp->GetCurrentWorldState());

	switch (Status)
	{
	case EHTNTaskStatus::Success:
		HTNComp->AdvancePlanIndex();
		return EBTNodeResult::Succeeded;

	case EHTNTaskStatus::Failed:
		HTNComp->InvalidatePlan();
		return EBTNodeResult::Failed;

	case EHTNTaskStatus::Running:
		return EBTNodeResult::InProgress;
	}

	return EBTNodeResult::Failed;
}
