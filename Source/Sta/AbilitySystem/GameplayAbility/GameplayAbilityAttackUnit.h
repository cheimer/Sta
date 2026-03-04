// Sta Project - Multiplay RTS with GAS

#pragma once

#include "CoreMinimal.h"
#include "StaGameplayAbility.h"
#include "GameplayAbilityAttackUnit.generated.h"

/**
 * 
 */
UCLASS()
class STA_API UGameplayAbilityAttackUnit : public UStaGameplayAbility
{
	GENERATED_BODY()
	
public:
	UGameplayAbilityAttackUnit();
	
	/**
	 * @param TriggerEventData TargetData[0]->GetActors()[0] : SrcArea
	 * @param TriggerEventData TargetData[0]->GetActors()[1] : DestArea
	 */
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Sta|Effect")
	TSubclassOf<UGameplayEffect> UnitChangeEffectClass;
	
};
