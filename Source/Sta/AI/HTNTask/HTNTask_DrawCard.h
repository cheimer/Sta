// Sta Project - Multiplay RTS with GAS

#pragma once

#include "CoreMinimal.h"
#include "HTNTaskBase.h"
#include "HTNTask_DrawCard.generated.h"

/**
 * 
 */
UCLASS()
class STA_API UHTNTask_DrawCard : public UHTNTaskBase
{
	GENERATED_BODY()

public:
	virtual bool CheckPrecondition(const FHTNWorldState& WS) const override;
	virtual void SimulateEffect(FHTNWorldState& WS) const override;
	virtual EHTNTaskStatus Execute(AStaAIController* Owner, const FHTNWorldState& WS) const override;

	float DrawCost = 0.0f;
	
};
