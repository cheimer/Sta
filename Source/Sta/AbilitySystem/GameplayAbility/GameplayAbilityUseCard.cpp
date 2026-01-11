// Sta Project - Multiplay RTS with GAS


#include "GameplayAbilityUseCard.h"

#include "DataAsset/CardData.h"
#include "GameplayTag/StaTags.h"
#include "Helper/StaHelper.h"

UGameplayAbilityUseCard::UGameplayAbilityUseCard()
{
	FAbilityTriggerData TriggerData;
	TriggerData.TriggerTag = StaTags::Event::Card::Use;
	TriggerData.TriggerSource = EGameplayAbilityTriggerSource::GameplayEvent;
	
	AbilityTriggers.Add(TriggerData);
}

void UGameplayAbilityUseCard::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	const UCardData* CardData = Cast<UCardData>(TriggerEventData->OptionalObject);
	if (!CardData) return;
	
	if (CardData->CardTag.MatchesAny(AbilityTags))
	{
		if (CardData->CardTag.MatchesTagExact(StaTags::Ability::Card::Spell))
		{
			SpellAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
		}
		else if (CardData->CardTag.MatchesTagExact(StaTags::Ability::Card::Employ))
		{
			EmployAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
		}
		else
		{
			check(false);
		}
	}
	
}

void UGameplayAbilityUseCard::SpellAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo& ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	StaDebug::Print(FString::Printf(TEXT("Spell Ability %s"), *GetName()));

	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}

void UGameplayAbilityUseCard::EmployAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo& ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	StaDebug::Print(FString::Printf(TEXT("Employ Ability %s"), *GetName()));
	
	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}
