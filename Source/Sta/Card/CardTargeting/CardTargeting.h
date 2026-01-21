// Sta Project - Multiplay RTS with GAS

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbilityTargetTypes.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "UObject/Object.h"
#include "CardTargeting.generated.h"

/**
 * 
 */
UCLASS(Abstract, Blueprintable, EditInlineNew, DefaultToInstanced)
class STA_API UCardTargeting : public UObject
{
	GENERATED_BODY()

public:
	virtual FGameplayAbilityTargetDataHandle FindTargets(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayEventData* EventData);
	
};
