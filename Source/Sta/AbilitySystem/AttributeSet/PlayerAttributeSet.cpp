// Sta Project - Multiplay RTS with GAS


#include "PlayerAttributeSet.h"

#include "GameplayEffectExtension.h"
#include "Helper/StaHelper.h"
#include "Net/UnrealNetwork.h"

UPlayerAttributeSet::UPlayerAttributeSet()
{
	InitCost(0.0f);
	InitMaxCost(10.0f);

	InitChargeTime(0.0f);
	InitChargeRate(1.0f);
	InitMaxCharge(10.0f);
}

void UPlayerAttributeSet::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, Cost, COND_OwnerOnly, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, MaxCost, COND_OwnerOnly, REPNOTIFY_Always);
	
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, ChargeTime, COND_OwnerOnly, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, ChargeRate, COND_OwnerOnly, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, MaxCharge, COND_OwnerOnly, REPNOTIFY_Always);
	
	
}

void UPlayerAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);
	
}

void UPlayerAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);
	
	if (Data.EvaluatedData.Attribute == GetCostAttribute())
	{
		float AdjustValue = FMath::Clamp(GetCost(), 0.0f, GetMaxCost());
		if (GetCost() != AdjustValue)
		{
			SetCost(AdjustValue);
		}
	}
	else if (Data.EvaluatedData.Attribute == GetChargeTimeAttribute())
	{
		float AdjustValue = FMath::Clamp(GetChargeTime(), 0.0f, GetMaxCharge());
		if (GetChargeTime() != AdjustValue)
		{
			SetChargeTime(AdjustValue);
		}
	}
}

void UPlayerAttributeSet::OnRep_Cost(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, Cost, OldValue);
}

void UPlayerAttributeSet::OnRep_MaxCost(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, MaxCost, OldValue);
}

void UPlayerAttributeSet::OnRep_ChargeTime(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, ChargeTime, OldValue);
}

void UPlayerAttributeSet::OnRep_ChargeRate(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, ChargeRate, OldValue);

}

void UPlayerAttributeSet::OnRep_MaxCharge(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, MaxCharge, OldValue);
}
