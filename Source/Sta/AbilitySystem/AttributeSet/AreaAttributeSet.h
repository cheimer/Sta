// Sta Project - Multiplay RTS with GAS

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "AreaAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

/**
 * 
 */
UCLASS()
class STA_API UAreaAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
	
public:
	UAreaAttributeSet();
	
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;

	ATTRIBUTE_ACCESSORS(ThisClass, UnitNum);
	ATTRIBUTE_ACCESSORS(ThisClass, BluffUnitAdd);
	ATTRIBUTE_ACCESSORS(ThisClass, MaxUnitNum);
	
	ATTRIBUTE_ACCESSORS(ThisClass, Defense);
	ATTRIBUTE_ACCESSORS(ThisClass, BluffDefenseAdd);
	
	UPROPERTY(BlueprintReadOnly)
	FGameplayAttributeData UnitNum;
	
	UPROPERTY(BlueprintReadOnly)
	FGameplayAttributeData BluffUnitAdd;
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxUnitNum)
	FGameplayAttributeData MaxUnitNum;
	
	UPROPERTY(BlueprintReadOnly)
	FGameplayAttributeData Defense;

	UPROPERTY(BlueprintReadOnly)
	FGameplayAttributeData BluffDefenseAdd;
	
protected:
	UFUNCTION()
	void OnRep_MaxUnitNum(const FGameplayAttributeData& OldValue);
	
};
