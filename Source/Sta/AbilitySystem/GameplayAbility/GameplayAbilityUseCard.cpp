// Sta Project - Multiplay RTS with GAS


#include "GameplayAbilityUseCard.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/AttributeSet/PlayerAttributeSet.h"
#include "Area/AreaBase.h"
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

	const UCardData* CardData = Cast<UCardData>(TriggerEventData->OptionalObject);
	if (!CardData)
	{
		CancelAbility(Handle, ActorInfo, ActivationInfo, true);
		return;
	}

	if (!CanUseCardCost(ActorInfo, CardData))
	{
		CancelAbility(Handle, ActorInfo, ActivationInfo, true);
		return;
	}

	if (CostGameplayEffectClass)
	{
		StaDebug::Print("CostGameplayEffectClass is not using. Should using CardCostEffectClass");
	}

	if (CardData->CardTag.MatchesAny(GetAssetTags()))
	{
		if (CardData->CardTag.MatchesTagExact(StaTags::Ability::Card::Spell))
		{
			SpellAbility(CardData, TriggerEventData->Target);
		}
		else if (CardData->CardTag.MatchesTagExact(StaTags::Ability::Card::Employ))
		{
			EmployAbility(CardData, TriggerEventData->Target);
		}
		else
		{
			CancelAbility(Handle, ActorInfo, ActivationInfo, true);
			return;
		}

		ApplyCardCost(ActorInfo, CardData);
	}
	
	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}

bool UGameplayAbilityUseCard::CanUseCardCost(const FGameplayAbilityActorInfo* ActorInfo, const UCardData* CardData)
{
	if (!CardData) return false;

	UAbilitySystemComponent* ASC = ActorInfo->AbilitySystemComponent.Get();
	if (!ASC) return false;

	float CurrentCost = ASC->GetNumericAttribute(UPlayerAttributeSet::GetCostAttribute());
	return CurrentCost >= CardData->Cost;
}

void UGameplayAbilityUseCard::ApplyCardCost(const FGameplayAbilityActorInfo* ActorInfo, const UCardData* CardData)
{
	if (!ActorInfo || !CardData || !CardCostEffectClass) return;
	
	UAbilitySystemComponent* ASC = ActorInfo->AbilitySystemComponent.Get();
	if (!ASC) return;

	FGameplayEffectSpecHandle CostSpecHandle = MakeOutgoingGameplayEffectSpec(CardCostEffectClass);
	if (!CostSpecHandle.IsValid() || !CostSpecHandle.Data.IsValid()) return;
	
	CostSpecHandle.Data->SetSetByCallerMagnitude(StaTags::SetByCaller::Cost, -CardData->Cost);
	
	ASC->ApplyGameplayEffectSpecToSelf(*CostSpecHandle.Data.Get());
}

void UGameplayAbilityUseCard::SpellAbility(const UCardData* CardData, const AActor* TargetActor)
{
	if (bShowDebug)
	{
		StaDebug::Print(FString::Printf(TEXT("%s Spell Ability"), *GetName()));
	}

}

void UGameplayAbilityUseCard::EmployAbility(const UCardData* CardData, const AActor* TargetActor)
{
	if (bShowDebug)
	{
		StaDebug::Print(FString::Printf(TEXT("%s Employ Ability"), *GetName()));
	}

	if (!CardData || !CardData->CardEffectClass) return;
	
	const IAbilitySystemInterface* AbilityInterface = Cast<IAbilitySystemInterface>(TargetActor);
	if (!AbilityInterface) return;

	UAbilitySystemComponent* TargetASC = AbilityInterface->GetAbilitySystemComponent();
	if (!TargetASC) return;

	FGameplayEffectSpecHandle CardEffectHandle = MakeOutgoingGameplayEffectSpec(CardData->CardEffectClass);
	if (!CardEffectHandle.IsValid() || !CardEffectHandle.Data.IsValid()) return;

	for (const FCardModifier& CardModifier : CardData->CardModifiers)
	{
		CardEffectHandle.Data->SetSetByCallerMagnitude(CardModifier.ValueTag, CardModifier.Value);
	}

	TargetASC->ApplyGameplayEffectSpecToSelf(*CardEffectHandle.Data.Get());
	
}
