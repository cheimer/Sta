// Sta Project - Multiplay RTS with GAS


#include "BTSer_StateCollecting.h"

#include "Component/HTNComponent.h"
#include "Component/HTNTypes.h"
#include "Controller/StaAIController.h"

UBTSer_StateCollecting::UBTSer_StateCollecting()
{
	NodeName = "State Collecting";
	Interval = 0.5f;
	RandomDeviation = 0.0f;
}

void UBTSer_StateCollecting::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	AStaAIController* Owner = Cast<AStaAIController>(OwnerComp.GetAIOwner());
	if (!Owner) return;

	UHTNComponent* HTN = Owner->FindComponentByClass<UHTNComponent>();
	if (!HTN) return;

	FHTNWorldState NewState = HTN->BuildWorldState(Owner);
	HTN->SetCurrentWorldState(NewState);
}
