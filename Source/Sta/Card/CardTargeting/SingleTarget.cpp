// Sta Project - Multiplay RTS with GAS


#include "SingleTarget.h"

#include "AbilitySystemInterface.h"

FGameplayAbilityTargetDataHandle USingleTarget::FindTargets(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayEventData* EventData)
{
	FGameplayAbilityTargetDataHandle DataHandle;
	
	AActor* Target = const_cast<AActor*>(EventData->Target.Get());
	if (!Target) return DataHandle;
	
	const IAbilitySystemInterface* AbilityInterface = Cast<IAbilitySystemInterface>(Target);
	if (!AbilityInterface || !AbilityInterface->GetAbilitySystemComponent()) return DataHandle;
	
	FGameplayAbilityTargetData_ActorArray* ActorArrayData = new FGameplayAbilityTargetData_ActorArray();
	ActorArrayData->TargetActorArray.Add(Target);
	
	DataHandle.Add(ActorArrayData);
	
	return DataHandle;
}
