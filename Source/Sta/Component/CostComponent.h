// Sta Project - Multiplay RTS with GAS

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CostComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class STA_API UCostComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UCostComponent();

protected:
	virtual void BeginPlay() override;

	
};
