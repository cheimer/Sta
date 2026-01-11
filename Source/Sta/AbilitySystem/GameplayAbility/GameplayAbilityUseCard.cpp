// Sta Project - Multiplay RTS with GAS


#include "GameplayAbilityUseCard.h"

#include "DataAsset/CardData.h"
#include "Helper/StaHelper.h"

void UGameplayAbilityUseCard::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                              const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	const UCardData* CardData = Cast<UCardData>(TriggerEventData->OptionalObject);
	if (!CardData) return;

	switch (CardData->CardType)
	{
	case ECardType::None:
		check(false);
		break;
	case ECardType::Spell:
		SpellAbility();
		break;
	case ECardType::Employ:
		EmployAbility();
		break;
	}
}

void UGameplayAbilityUseCard::SpellAbility()
{
	StaDebug::Print(FString::Printf(TEXT("Spell Ability %s"), *GetName()));
}

void UGameplayAbilityUseCard::EmployAbility()
{
	StaDebug::Print(FString::Printf(TEXT("Employ Ability %s"), *GetName()));
}
