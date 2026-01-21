// Sta Project - Multiplay RTS with GAS

#pragma once

#include "CoreMinimal.h"
#include "StaGameplayAbility.h"
#include "GameplayAbilityDrawCard.generated.h"

/**
 * 
 */
UCLASS()
class STA_API UGameplayAbilityDrawCard : public UStaGameplayAbility
{
	GENERATED_BODY()

public:
	UGameplayAbilityDrawCard();
	
	/**
	 * @param TriggerEventData Target : Playing Pawn
	 */
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

protected:
	void ChargeTimeReset();

	UPROPERTY(EditDefaultsOnly, Category = "Sta|Effect")
	TSubclassOf<UGameplayEffect> ChargeResetEffect;
	
};
