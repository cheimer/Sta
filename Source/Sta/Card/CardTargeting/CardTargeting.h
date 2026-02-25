// Sta Project - Multiplay RTS with GAS

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbilityTargetTypes.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "UObject/Object.h"
#include "CardTargeting.generated.h"

UENUM()
enum class ETargetFilter
{
	Any,
	Friendly,
	Hostile,
	NoTeam
};

/**
 * 
 */
UCLASS(Abstract, Blueprintable, EditInlineNew, DefaultToInstanced)
class STA_API UCardTargeting : public UObject
{
	GENERATED_BODY()

public:
	virtual FGameplayAbilityTargetDataHandle FindTargets(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayEventData* EventData);

protected:
	virtual bool IsTargetPassFilter(const FGameplayAbilityActorInfo* ActorInfo, AActor* TargetActor);
	
	UPROPERTY(EditAnywhere, Category = "Targeting")
	ETargetFilter TargetFilter = ETargetFilter::Any;
	
};
