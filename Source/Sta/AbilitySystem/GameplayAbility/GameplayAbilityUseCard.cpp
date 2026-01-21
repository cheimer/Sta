// Sta Project - Multiplay RTS with GAS


#include "GameplayAbilityUseCard.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/AttributeSet/PlayerAttributeSet.h"
#include "Area/AreaBase.h"
#include "Card/CardTargeting/CardTargeting.h"
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

	if (CostGameplayEffectClass)
	{
		StaDebug::Print("CostGameplayEffectClass is not using. Should using CardCostEffectClass");
	}

	const UCardData* CardData = Cast<UCardData>(TriggerEventData->OptionalObject);
	if (!CardData || !CardData->CardTag.MatchesAny(GetAssetTags()))
	{
		CancelAbility(Handle, ActorInfo, ActivationInfo, true);
		return;
	}
	
	if (!CanUseCardCost(CardData))
	{
		CancelAbility(Handle, ActorInfo, ActivationInfo, true);
		return;
	}

	FGameplayAbilityTargetDataHandle TargetDataHandle = CardData->CardTargeting->FindTargets(GetCurrentActorInfo(), TriggerEventData);
	if ( TargetDataHandle.Num() <= 0)
	{
		CancelAbility(Handle, ActorInfo, ActivationInfo, true);
	}

	ApplyCardCost(CardData);
	ActivateCardAbility(CardData, TargetDataHandle);
	
	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}

bool UGameplayAbilityUseCard::CanUseCardCost(const UCardData* CardData)
{
	if (!CardData) return false;

	UAbilitySystemComponent* OwnerASC = GetCurrentActorInfo()->AbilitySystemComponent.Get();
	if (!OwnerASC) return false;

	float CurrentCost = OwnerASC->GetNumericAttribute(UPlayerAttributeSet::GetCostAttribute());
	
	return CurrentCost >= CardData->Cost;
}

void UGameplayAbilityUseCard::ApplyCardCost(const UCardData* CardData)
{
	if (!CardData || !CardCostEffectClass) return;
	
	FGameplayEffectSpecHandle CostSpecHandle = MakeOutgoingGameplayEffectSpec(CardCostEffectClass);
	if (!CostSpecHandle.IsValid() || !CostSpecHandle.Data.IsValid()) return;

	CostSpecHandle.Data->SetSetByCallerMagnitude(StaTags::SetByCaller::Cost, -CardData->Cost);
	
	ApplyGameplayEffectSpecToOwner(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), CostSpecHandle);
}

void UGameplayAbilityUseCard::ActivateCardAbility(const UCardData* CardData, const FGameplayAbilityTargetDataHandle TargetDataHandle)
{
	if (bShowDebug)
	{
		StaDebug::Print(FString::Printf(TEXT("%s Activate Ability"), *GetName()));
	}
	
	if (!CardData || !CardData->CardEffectClass) return;

	FGameplayEffectSpecHandle CardEffectHandle = MakeOutgoingGameplayEffectSpec(CardData->CardEffectClass);
	if (!CardEffectHandle.IsValid() || !CardEffectHandle.Data.IsValid()) return;

	for (const FCardModifier& CardModifier : CardData->CardModifiers)
	{
		CardEffectHandle.Data->SetSetByCallerMagnitude(CardModifier.ValueTag, CardModifier.Value);
	}

	ApplyGameplayEffectSpecToTarget(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), CardEffectHandle, TargetDataHandle);
	
}
