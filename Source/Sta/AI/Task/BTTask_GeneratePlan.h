// Sta Project - Multiplay RTS with GAS

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_GeneratePlan.generated.h"

/**
 * 
 */
UCLASS()
class STA_API UBTTask_GeneratePlan : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_GeneratePlan();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
};
