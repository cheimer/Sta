// Sta Project - Multiplay RTS with GAS


#include "PlayerAttributeSet.h"

#include "Net/UnrealNetwork.h"

UPlayerAttributeSet::UPlayerAttributeSet()
{
	InitCost(0.0f);
	InitMaxCost(10.0f);
}

void UPlayerAttributeSet::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, Cost, COND_OwnerOnly, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, MaxCost, COND_OwnerOnly, REPNOTIFY_Always);
	
}

void UPlayerAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if (Attribute == GetCostAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxCost());
	}
}

void UPlayerAttributeSet::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);
}

void UPlayerAttributeSet::OnRep_Cost(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, Cost, OldValue);
}

void UPlayerAttributeSet::OnRep_MaxCost(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, MaxCost, OldValue);
}
