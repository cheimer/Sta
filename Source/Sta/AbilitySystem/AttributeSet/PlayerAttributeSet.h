// Sta Project - Multiplay RTS with GAS

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "PlayerAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

/**
 * 
 */
UCLASS()
class STA_API UPlayerAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	UPlayerAttributeSet();
	
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;

	ATTRIBUTE_ACCESSORS(ThisClass, Cost);
	ATTRIBUTE_ACCESSORS(ThisClass, MaxCost);
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Cost)
	FGameplayAttributeData Cost;
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxCost)
	FGameplayAttributeData MaxCost;

protected:
	UFUNCTION()
	void OnRep_Cost(const FGameplayAttributeData& OldValue);
	UFUNCTION()
	void OnRep_MaxCost(const FGameplayAttributeData& OldValue);
	
};
