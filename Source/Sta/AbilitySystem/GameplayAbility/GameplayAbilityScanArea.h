// Sta Project - Multiplay RTS with GAS

#pragma once

#include "CoreMinimal.h"
#include "StaGameplayAbility.h"
#include "GameplayAbilityScanArea.generated.h"

/**
 * 
 */
UCLASS()
class STA_API UGameplayAbilityScanArea : public UStaGameplayAbility
{
	GENERATED_BODY()
	
public:
	UGameplayAbilityScanArea();
	
	/**
	 * @param TriggerEventData TargetData[0]->GetActors()[0] : Target Area
	 */
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

};
