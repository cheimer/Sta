// Sta Project - Multiplay RTS with GAS


#include "StaAbilitySystemComponent.h"

void UStaAbilitySystemComponent::ServerHandleGameplayEvent_Implementation(FGameplayTag GameplayTag, const FGameplayEventData& EventData)
{
	HandleGameplayEvent(GameplayTag, &EventData);
}
