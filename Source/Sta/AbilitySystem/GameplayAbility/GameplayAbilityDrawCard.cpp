// Sta Project - Multiplay RTS with GAS


#include "GameplayAbilityDrawCard.h"

#include "AbilitySystemComponent.h"
#include "GameplayTag/StaTags.h"
#include "Helper/StaHelper.h"

UGameplayAbilityDrawCard::UGameplayAbilityDrawCard()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	bRetriggerInstancedAbility = false;
	
	FAbilityTriggerData TriggerData;
	TriggerData.TriggerTag = StaTags::Event::Card::Draw;
	TriggerData.TriggerSource = EGameplayAbilityTriggerSource::GameplayEvent;
	
	AbilityTriggers.Add(TriggerData);
}

void UGameplayAbilityDrawCard::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (!ChargeResetEffect)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
		return;
	}

	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
		return;
	}
	
	ChargeTimeReset();

	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}

void UGameplayAbilityDrawCard::ChargeTimeReset()
{
	FGameplayEffectSpecHandle EffectSpec = MakeOutgoingGameplayEffectSpec(ChargeResetEffect);
	if (!EffectSpec.IsValid()) return;
	
	ApplyGameplayEffectSpecToOwner(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), EffectSpec);
}
