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

	/**
	 * @param TriggerEventData Instigator : Owner Actor
	 * @param TriggerEventData OptionalObject : UCardData DataAsset
	 * @param TriggerEventData Target : Area Actor
	 */
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	                             const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

protected:
	bool CanUseCardCost(const UCardData* CardData);
	void ApplyCardCost(const UCardData* CardData);

	virtual void ActivateCardAbility(const UCardData* CardData, const FGameplayAbilityTargetDataHandle TargetDataHandle);

	UPROPERTY(EditDefaultsOnly, Category = "Sta|Effect")
	TSubclassOf<UGameplayEffect> CardCostEffectClass;
	
};
