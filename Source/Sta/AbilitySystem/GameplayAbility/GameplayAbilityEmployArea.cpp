// Sta Project - Multiplay RTS with GAS


#include "GameplayAbilityEmployArea.h"

#include "AbilitySystemComponent.h"
#include "Area/AreaBase.h"
#include "GameplayTag/StaTags.h"
#include "Subsystem/NotifySubsystem.h"

UGameplayAbilityEmployArea::UGameplayAbilityEmployArea()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	bRetriggerInstancedAbility = true;

	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::ServerOnly;
	
	FAbilityTriggerData TriggerData;
	TriggerData.TriggerTag = StaTags::Event::Area::Employ;
	TriggerData.TriggerSource = EGameplayAbilityTriggerSource::GameplayEvent;
	
	AbilityTriggers.Add(TriggerData);
}

void UGameplayAbilityEmployArea::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	if (!GetCostGameplayEffect() || !UnitChangeEffectClass)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}
	
	const FGameplayAbilityTargetData* Data = TriggerEventData->TargetData.Get(0);
	if (!Data)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}
	
	TArray<TWeakObjectPtr<AActor>> AreaActors = Data->GetActors();
	if (AreaActors.Num() < 1 || !AreaActors[0].IsValid())
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	AAreaBase* TargetArea = Cast<AAreaBase>(AreaActors[0].Get());
	if (!TargetArea || !TargetArea->GetAbilitySystemComponent())
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	if (TriggerEventData->EventMagnitude < 0.0f)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	FGameplayEffectSpecHandle UnitEffectHandle = MakeOutgoingGameplayEffectSpec(UnitChangeEffectClass);
	if (!UnitEffectHandle.IsValid() || !UnitEffectHandle.Data.IsValid()) return;
	
	UnitEffectHandle.Data->SetSetByCallerMagnitude(StaTags::SetByCaller::UnitNum, TriggerEventData->EventMagnitude);
	TargetArea->GetAbilitySystemComponent()->ApplyGameplayEffectSpecToSelf(*UnitEffectHandle.Data.Get());
	
	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
	
}
