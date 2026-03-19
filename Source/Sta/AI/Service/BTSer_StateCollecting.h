// Sta Project - Multiplay RTS with GAS

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTSer_StateCollecting.generated.h"

/**
 * 
 */
UCLASS()
class STA_API UBTSer_StateCollecting : public UBTService
{
	GENERATED_BODY()

public:
	UBTSer_StateCollecting();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

};
