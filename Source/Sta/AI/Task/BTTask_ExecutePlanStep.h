// Sta Project - Multiplay RTS with GAS

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_ExecutePlanStep.generated.h"

/**
 * 
 */
UCLASS()
class STA_API UBTTask_ExecutePlanStep : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_ExecutePlanStep();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
};
