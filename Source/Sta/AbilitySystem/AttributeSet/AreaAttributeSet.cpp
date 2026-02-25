// Sta Project - Multiplay RTS with GAS


#include "AreaAttributeSet.h"

#include "Net/UnrealNetwork.h"

UAreaAttributeSet::UAreaAttributeSet()
{
	InitUnitNum(0.0f);
	InitBluffUnitAdd(0.0f);
	InitMaxUnitNum(99.0f);
	
	InitDefense(1.5f);
	InitBluffDefenseAdd(0.0f);
}

void UAreaAttributeSet::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(ThisClass, MaxUnitNum);
	
}

void UAreaAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if (Attribute == GetUnitNumAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxUnitNum());
	}
}

void UAreaAttributeSet::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	
}

void UAreaAttributeSet::OnRep_MaxUnitNum(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, MaxUnitNum, OldValue);
}
