// Sta Project - Multiplay RTS with GAS

#pragma once

#include "CoreMinimal.h"
#include "StaGameplayAbility.h"
#include "GameplayAbilityUseCard.generated.h"

/**
 * 
 */
UCLASS()
class STA_API UGameplayAbilityUseCard : public UStaGameplayAbility
{
	GENERATED_BODY()

public:
	UGameplayAbilityUseCard();
	
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

protected:
	void SpellAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo& ActivationInfo, const FGameplayEventData* TriggerEventData);
	void EmployAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo& ActivationInfo, const FGameplayEventData* TriggerEventData);
	
};
