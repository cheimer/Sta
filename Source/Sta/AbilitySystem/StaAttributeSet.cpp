// Sta Project - Multiplay RTS with GAS


#include "StaAttributeSet.h"

#include "Net/UnrealNetwork.h"

UStaAttributeSet::UStaAttributeSet()
{
	InitCost(0.0f);
	InitMaxCost(10.0f);
}

void UStaAttributeSet::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, Cost, COND_OwnerOnly, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, MaxCost, COND_OwnerOnly, REPNOTIFY_Always);
	
}

void UStaAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if (Attribute == GetCostAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxCost());
	}
}

void UStaAttributeSet::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);
}

void UStaAttributeSet::OnRep_Cost(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, Cost, OldValue);
}

void UStaAttributeSet::OnRep_MaxCost(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, MaxCost, OldValue);
}
