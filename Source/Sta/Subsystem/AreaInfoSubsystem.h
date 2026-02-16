// Sta Project - Multiplay RTS with GAS

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "AreaInfoSubsystem.generated.h"

class AAreaBase;
/**
 * 
 */
UCLASS()
class STA_API UAreaInfoSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	virtual void OnWorldBeginPlay(UWorld& InWorld) override;

private:
	void OnAreaValueChanged(AAreaBase* AreaActor, const float UnitValue, const float DefenseValue);
	
};
