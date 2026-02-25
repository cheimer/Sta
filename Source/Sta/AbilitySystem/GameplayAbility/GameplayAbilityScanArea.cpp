// Sta Project - Multiplay RTS with GAS


#include "GameplayAbilityScanArea.h"

#include "Area/AreaBase.h"
#include "GameFramework/PlayerState.h"
#include "GameplayTag/StaTags.h"
#include "Subsystem/AreaInfoSubsystem.h"

UGameplayAbilityScanArea::UGameplayAbilityScanArea()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	bRetriggerInstancedAbility = true;

	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::ServerOnly;
	
	FAbilityTriggerData TriggerData;
	TriggerData.TriggerTag = StaTags::Event::Area::Scan;
	TriggerData.TriggerSource = EGameplayAbilityTriggerSource::GameplayEvent;
	
	AbilityTriggers.Add(TriggerData);
}

void UGameplayAbilityScanArea::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (!GetCostGameplayEffect())
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
	APlayerState* PS = Cast<APlayerState>(GetOwningActorFromActorInfo());
	UAreaInfoSubsystem* AreaInfoSubsystem = GetWorld()->GetSubsystem<UAreaInfoSubsystem>();
	if (!TargetArea || !PS || !AreaInfoSubsystem)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	AreaInfoSubsystem->UpdateAreaBluffTo(TargetArea, PS);
	
	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}
