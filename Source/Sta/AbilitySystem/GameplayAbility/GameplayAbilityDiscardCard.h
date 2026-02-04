// Sta Project - Multiplay RTS with GAS

#pragma once

#include "CoreMinimal.h"
#include "StaGameplayAbility.h"
#include "GameplayAbilityDiscardCard.generated.h"

/**
 * 
 */
UCLASS()
class STA_API UGameplayAbilityDiscardCard : public UStaGameplayAbility
{
	GENERATED_BODY()
	
public:
	UGameplayAbilityDiscardCard();
	
	/**
	 * @param TriggerEventData Instigator : Owner Actor
	 * @param TriggerEventData OptionalObject : UCardData DataAsset
	 */
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;


};
