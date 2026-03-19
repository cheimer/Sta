// Sta Project - Multiplay RTS with GAS

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDeco_NeedRePlan.generated.h"

/**
 * 
 */
UCLASS()
class STA_API UBTDeco_NeedRePlan : public UBTDecorator
{
	GENERATED_BODY()

public:
	UBTDeco_NeedRePlan();

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
	
	UPROPERTY(EditAnywhere, Category = "Sta|Value")
	float UnitChangeThreshold = 0.3f;
	
};
