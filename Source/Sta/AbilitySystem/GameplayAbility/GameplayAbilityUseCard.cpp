// Sta Project - Multiplay RTS with GAS


#include "GameplayAbilityUseCard.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/AttributeSet/PlayerAttributeSet.h"
#include "Card/CardBase.h"
#include "DataAsset/CardData.h"
#include "GameplayTag/StaTags.h"
#include "Helper/StaHelper.h"

UGameplayAbilityUseCard::UGameplayAbilityUseCard()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	bRetriggerInstancedAbility = false;
	
	FAbilityTriggerData TriggerData;
	TriggerData.TriggerTag = StaTags::Event::Card::Use;
	TriggerData.TriggerSource = EGameplayAbilityTriggerSource::GameplayEvent;
	
	AbilityTriggers.Add(TriggerData);
}

void UGameplayAbilityUseCard::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	const ACardBase* CardActor = Cast<ACardBase>(TriggerEventData->OptionalObject);
	if (!CardActor || !CardActor->GetCardData())
	{
		CancelAbility(Handle, ActorInfo, ActivationInfo, true);
		return;
	}

	if (!CanUseCardCost(ActorInfo, CardActor->GetCardData()))
	{
		CancelAbility(Handle, ActorInfo, ActivationInfo, true);
		return;
	}
	
	CurrentCardData = CardActor->GetCardData();
	if (CurrentCardData->CardTag.MatchesAny(GetAssetTags()))
	{
		if (CurrentCardData->CardTag.MatchesTagExact(StaTags::Ability::Card::Spell))
		{
			SpellAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
		}
		else if (CurrentCardData->CardTag.MatchesTagExact(StaTags::Ability::Card::Employ))
		{
			EmployAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
		}
		else
		{
			CancelAbility(Handle, ActorInfo, ActivationInfo, true);
			return;
		}

		ApplyCardCost(ActorInfo);
	}
	
}

bool UGameplayAbilityUseCard::CanUseCardCost(const FGameplayAbilityActorInfo* ActorInfo, const UCardData* CardData)
{
	if (!CardData) return false;

	UAbilitySystemComponent* ASC = ActorInfo->AbilitySystemComponent.Get();
	if (!ASC) return false;

	float CurrentCost = ASC->GetNumericAttribute(UPlayerAttributeSet::GetCostAttribute());
	return CurrentCost >= CardData->Cost;
}

void UGameplayAbilityUseCard::ApplyCardCost(const FGameplayAbilityActorInfo* ActorInfo)
{
	if (!ActorInfo || !CurrentCardData || !CurrentCardData->CardEffectClass) return;
	
	UAbilitySystemComponent* ASC = ActorInfo->AbilitySystemComponent.Get();
	if (!ASC) return;

	FGameplayEffectSpecHandle CostSpecHandle = MakeOutgoingGameplayEffectSpec(CurrentCardData->CardEffectClass);
	if (!CostSpecHandle.IsValid()) return;
	
	CostSpecHandle.Data->SetSetByCallerMagnitude(StaTags::SetByCaller::Cost, -CurrentCardData->Cost);
	
	ASC->ApplyGameplayEffectSpecToSelf(*CostSpecHandle.Data.Get());
}

void UGameplayAbilityUseCard::SpellAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo& ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (bShowDebug)
	{
		StaDebug::Print(FString::Printf(TEXT("Spell Ability %s"), *GetName()));
	}
	

	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}

void UGameplayAbilityUseCard::EmployAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo& ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (bShowDebug)
	{
		StaDebug::Print(FString::Printf(TEXT("Employ Ability %s"), *GetName()));
	}
	
	
	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}
