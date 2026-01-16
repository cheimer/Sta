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
	 * 
	 * @param TriggerEventData OptionalObject : UCardData DataAsset
	 * @param TriggerEventData Target : Area Actor
	 * 
	 */
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	                             const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

protected:
	bool CanUseCardCost(const FGameplayAbilityActorInfo* ActorInfo, const UCardData* CardData);
	void ApplyCardCost(const FGameplayAbilityActorInfo* ActorInfo, const UCardData* CardData);
	
	virtual void SpellAbility(const UCardData* CardData, const AActor* TargetActor);
	virtual void EmployAbility(const UCardData* CardData, const AActor* TargetActor);

	UPROPERTY(EditDefaultsOnly, Category = "Sta|Effect")
	TSubclassOf<UGameplayEffect> CardCostEffectClass;
	
};
