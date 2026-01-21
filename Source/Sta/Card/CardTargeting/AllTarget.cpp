// Sta Project - Multiplay RTS with GAS


#include "AllTarget.h"

#include "AbilitySystemInterface.h"
#include "Area/AreaBase.h"
#include "Kismet/GameplayStatics.h"

FGameplayAbilityTargetDataHandle UAllTarget::FindTargets(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayEventData* EventData)
{
	FGameplayAbilityTargetDataHandle DataHandle;
	
	TArray<AActor*> FoundTargets;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AAreaBase::StaticClass(), FoundTargets);

	FGameplayAbilityTargetData_ActorArray* ActorArrayData = new FGameplayAbilityTargetData_ActorArray();
	
	for (AActor* Target : FoundTargets)
	{
		const IAbilitySystemInterface* AbilityInterface = Cast<IAbilitySystemInterface>(Target);
		if (!AbilityInterface || !AbilityInterface->GetAbilitySystemComponent()) continue;
	
		ActorArrayData->TargetActorArray.Add(Target);
	}
	DataHandle.Add(ActorArrayData);
	
	return DataHandle;
}
