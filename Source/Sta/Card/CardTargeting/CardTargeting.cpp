// Sta Project - Multiplay RTS with GAS


#include "CardTargeting.h"

#include "GenericTeamAgentInterface.h"

FGameplayAbilityTargetDataHandle UCardTargeting::FindTargets(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayEventData* EventData)
{
	return FGameplayAbilityTargetDataHandle();
}

bool UCardTargeting::IsTargetPassFilter(const FGameplayAbilityActorInfo* ActorInfo, AActor* TargetActor)
{
	if (TargetFilter == ETargetFilter::Any) return true;
	
	if (!TargetActor) return false;

	IGenericTeamAgentInterface* TargetTeam = Cast<IGenericTeamAgentInterface>(TargetActor);
	if (!TargetTeam) return false;

	IGenericTeamAgentInterface* OwnerTeam = Cast<IGenericTeamAgentInterface>(ActorInfo->OwnerActor);
	if (!OwnerTeam) return false;

	if (TargetFilter == ETargetFilter::NoTeam)
	{
		return TargetTeam->GetGenericTeamId() == FGenericTeamId::NoTeam;
	}
	else if (TargetFilter == ETargetFilter::Friendly)
	{
		return TargetTeam->GetGenericTeamId() == OwnerTeam->GetGenericTeamId();
	}
	else if (TargetFilter == ETargetFilter::Hostile)
	{
		return TargetTeam->GetGenericTeamId() != OwnerTeam->GetGenericTeamId();
	}
	else
	{
		check(false);
	}

	return false;
}
