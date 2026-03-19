// Sta Project - Multiplay RTS with GAS

#pragma once

#include "CoreMinimal.h"
#include "StaGameplayAbility.h"
#include "GameplayAbilityEmployArea.generated.h"

/**
 * 
 */
UCLASS()
class STA_API UGameplayAbilityEmployArea : public UStaGameplayAbility
{
	GENERATED_BODY()
	
public:	
	UGameplayAbilityEmployArea();
	
	/**
	 * @param TriggerEventData TargetData[0]->GetActors()[0] : Target Area
	 * @param TriggerEventData EventMagnitude : UnitNum
	 */
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Sta|Effect")
	TSubclassOf<UGameplayEffect> UnitChangeEffectClass;
	
};
