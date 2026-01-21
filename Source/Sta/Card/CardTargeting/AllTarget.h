// Sta Project - Multiplay RTS with GAS

#pragma once

#include "CoreMinimal.h"
#include "CardTargeting.h"
#include "AllTarget.generated.h"

/**
 * 
 */
UCLASS()
class STA_API UAllTarget : public UCardTargeting
{
	GENERATED_BODY()
	
public:
	virtual FGameplayAbilityTargetDataHandle FindTargets(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayEventData* EventData) override;
	
};
