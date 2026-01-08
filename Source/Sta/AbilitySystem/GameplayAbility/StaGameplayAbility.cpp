// Sta Project - Multiplay RTS with GAS


#include "StaGameplayAbility.h"

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
