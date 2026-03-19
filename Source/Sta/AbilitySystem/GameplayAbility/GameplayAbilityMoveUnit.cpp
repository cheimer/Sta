// Sta Project - Multiplay RTS with GAS


#include "GameplayAbilityMoveUnit.h"

#include "AbilitySystemComponent.h"
#include "Area/AreaBase.h"
#include "GameplayTag/StaTags.h"
#include "Helper/StaHelper.h"

UGameplayAbilityMoveUnit::UGameplayAbilityMoveUnit()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	bRetriggerInstancedAbility = false;

	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::ServerOnly;
	
	FAbilityTriggerData TriggerData;
	TriggerData.TriggerTag = StaTags::Event::Area::Move;
	TriggerData.TriggerSource = EGameplayAbilityTriggerSource::GameplayEvent;
	
	AbilityTriggers.Add(TriggerData);
}

void UGameplayAbilityMoveUnit::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (!UnitChangeEffectClass)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}
	
	float UnitNum = TriggerEventData->EventMagnitude;
	
	const FGameplayAbilityTargetData* Data = TriggerEventData->TargetData.Get(0);
	if (!Data)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}
	
	TArray<TWeakObjectPtr<AActor>> AreaActors = Data->GetActors();
	if (AreaActors.Num() < 2 || !AreaActors[0].IsValid() || !AreaActors[1].IsValid())
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	AAreaBase* SrcArea = Cast<AAreaBase>(AreaActors[0].Get());
	AAreaBase* DestArea = Cast<AAreaBase>(AreaActors[1].Get());
	if (!SrcArea || !DestArea || !SrcArea->GetAbilitySystemComponent() || !DestArea->GetAbilitySystemComponent())
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	if (SrcArea->GetGenericTeamId() != DestArea->GetGenericTeamId())
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}
	
	FGameplayEffectSpecHandle SrcUnitSpecHandle = MakeOutgoingGameplayEffectSpec(UnitChangeEffectClass);
	FGameplayEffectSpecHandle DestUnitSpecHandle = MakeOutgoingGameplayEffectSpec(UnitChangeEffectClass);
	if (!SrcUnitSpecHandle.IsValid() || !SrcUnitSpecHandle.Data.IsValid() || !DestUnitSpecHandle.IsValid() || !DestUnitSpecHandle.Data.IsValid())
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}
	
	SrcUnitSpecHandle.Data->SetSetByCallerMagnitude(StaTags::SetByCaller::UnitNum, -UnitNum);
	DestUnitSpecHandle.Data->SetSetByCallerMagnitude(StaTags::SetByCaller::UnitNum, UnitNum);

	SrcArea->GetAbilitySystemComponent()->ApplyGameplayEffectSpecToSelf(*SrcUnitSpecHandle.Data.Get());
	DestArea->GetAbilitySystemComponent()->ApplyGameplayEffectSpecToSelf(*DestUnitSpecHandle.Data.Get());

	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
	
}
