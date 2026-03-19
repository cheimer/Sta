// Sta Project - Multiplay RTS with GAS


#include "BTTask_GeneratePlan.h"

#include "Component/HTNComponent.h"
#include "Controller/StaAIController.h"

UBTTask_GeneratePlan::UBTTask_GeneratePlan()
{
	NodeName = "Generate HTN Plan";
}

EBTNodeResult::Type UBTTask_GeneratePlan::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	AStaAIController* OwnerController = Cast<AStaAIController>(OwnerComp.GetAIOwner());
	if (!OwnerController) return EBTNodeResult::Failed;

	UHTNComponent* HTNComp = OwnerController->FindComponentByClass<UHTNComponent>();
	if (!HTNComp) return EBTNodeResult::Failed;

	bool bSuccess = HTNComp->RequestGeneratePlan(OwnerController->GetDeckData());
	
	return bSuccess ? EBTNodeResult::Succeeded : EBTNodeResult::Failed;
}
