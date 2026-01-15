// Sta Project - Multiplay RTS with GAS

#pragma once

#include "CoreMinimal.h"
#include "StaGameplayAbility.h"
#include "GameplayAbilityUseCard.generated.h"

class UCardData;
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
	bool CanUseCardCost(const FGameplayAbilityActorInfo* ActorInfo, const UCardData* CardData);
	void ApplyCardCost(const FGameplayAbilityActorInfo* ActorInfo);
	
	virtual void SpellAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo& ActivationInfo, const FGameplayEventData* TriggerEventData);
	virtual void EmployAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo& ActivationInfo, const FGameplayEventData* TriggerEventData);

	UPROPERTY(Transient)
	UCardData* CurrentCardData;
	
};
