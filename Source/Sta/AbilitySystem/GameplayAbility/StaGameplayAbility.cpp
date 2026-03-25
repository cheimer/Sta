// Sta Project - Multiplay RTS with GAS


#include "StaGameplayAbility.h"

#include "Subsystem/NotifySubsystem.h"

void UStaGameplayAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                          const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (bShowDebug && IsValid(GEngine))
	{
		if (ActorInfo->AvatarActor.IsValid())
		{
			GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green,
				FString::Printf(TEXT("%s activate ability from %s"), *GetName(), *ActorInfo->AvatarActor->GetName()));
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green,
				FString::Printf(TEXT("?? activate ability %s"), *GetName()));
		}
		
	}
}

bool UStaGameplayAbility::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags,
	FGameplayTagContainer* OptionalRelevantTags) const
{
	bool bIsCanActivate = Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);

	if (!bIsCanActivate)
	{
		NotifyToHUD(ActorInfo);
	}
	
	return bIsCanActivate;
}

void UStaGameplayAbility::NotifyToHUD(const FGameplayAbilityActorInfo* ActorInfo) const
{
	if (UNotifySubsystem* NotifySubsystem = GetWorld()->GetSubsystem<UNotifySubsystem>())
	{
		if (APawn* AvatarPawn = Cast<APawn>(ActorInfo->AvatarActor.Get()))
		{
			NotifySubsystem->NotifyCostNeed(ENotifyPriority::Low, AvatarPawn->GetController());
		}
	}
}
