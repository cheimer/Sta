// Sta Project - Multiplay RTS with GAS

#pragma once

#include "CoreMinimal.h"
#include "Component/HTNTypes.h"
#include "UObject/Object.h"
#include "HTNTaskBase.generated.h"

class AStaAIController;
/**
 * 
 */
UCLASS(Abstract)
class STA_API UHTNTaskBase : public UObject
{
	GENERATED_BODY()

public:
	virtual bool CheckPrecondition(const FHTNWorldState& WS) const {return false;}
	virtual void SimulateEffect(FHTNWorldState& WS) const {}
	virtual EHTNTaskStatus Execute(AStaAIController* Owner, const FHTNWorldState& WS) const {return EHTNTaskStatus::Failed;}

	int32 SourceAreaIndex = INDEX_NONE;
	int32 TargetAreaIndex = INDEX_NONE;
	float UnitCount = 0.f;
	int32 CardIndex = INDEX_NONE;

protected:
	UPROPERTY(EditAnywhere, Category = "Sta|Debug")
	bool bShowDebug = false;
	
};
