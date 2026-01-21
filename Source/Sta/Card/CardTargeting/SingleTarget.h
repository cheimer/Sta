// Sta Project - Multiplay RTS with GAS

#pragma once

#include "CoreMinimal.h"
#include "CardTargeting.h"
#include "SingleTarget.generated.h"

/**
 * 
 */
UCLASS()
class STA_API USingleTarget : public UCardTargeting
{
	GENERATED_BODY()

public:
	virtual FGameplayAbilityTargetDataHandle FindTargets(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayEventData* EventData) override;
	
};
