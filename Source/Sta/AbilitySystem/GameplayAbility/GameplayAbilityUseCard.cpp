// Sta Project - Multiplay RTS with GAS


#include "GameplayAbilityUseCard.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/AttributeSet/PlayerAttributeSet.h"
#include "Area/AreaBase.h"
#include "Card/CardTargeting/CardTargeting.h"
#include "DataAsset/CardData.h"
#include "Framework/GameMode/StaGameModeBase.h"
#include "GameFramework/PlayerState.h"
#include "GameplayTag/StaTags.h"
#include "Helper/StaHelper.h"

UGameplayAbilityUseCard::UGameplayAbilityUseCard()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	bRetriggerInstancedAbility = false;
	
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
	
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

	AStaGameModeBase* GameMode = GetWorld()->GetAuthGameMode<AStaGameModeBase>();
	APlayerState* PS = Cast<APlayerState>(GetOwningActorFromActorInfo());

	if (!GameMode || !PS)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
		return;
	}

	AController* PC = Cast<AController>(PS->GetOwner());
	if (!PC)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
		return;
	}

	const UCardData* CardData = Cast<UCardData>(TriggerEventData->OptionalObject);
	if (!CardData || !CardData->CardTag.MatchesAny(GetAssetTags()))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}
	
	if (!CanUseCardCost(CardData))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	FGameplayAbilityTargetDataHandle TargetDataHandle = CardData->CardTargeting->FindTargets(GetCurrentActorInfo(), TriggerEventData);
	if (TargetDataHandle.Num() <= 0)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	if (!GameMode->CardInHand(PC, CardData))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	ApplyCardCost(CardData);
	ActivateCardAbility(CardData, TargetDataHandle);
	
	GameMode->DiscardCard(PC, CardData);
	
	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}

bool UGameplayAbilityUseCard::CanUseCardCost(const UCardData* CardData)
{
	if (!CardData) return false;

	UAbilitySystemComponent* OwnerASC = GetCurrentActorInfo()->AbilitySystemComponent.Get();
	if (!OwnerASC) return false;

	float CurrentCost = OwnerASC->GetNumericAttribute(UPlayerAttributeSet::GetCostAttribute());
	
	bool bIsCanUseCard = CurrentCost >= CardData->Cost;
	if (!bIsCanUseCard)
	{
		NotifyToHUD(GetCurrentActorInfo());
	}

	return bIsCanUseCard;
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
	if (!CardData || !CardData->CardEffectClass) return;

	FGameplayEffectSpecHandle CardEffectHandle = MakeOutgoingGameplayEffectSpec(CardData->CardEffectClass);
	if (!CardEffectHandle.IsValid() || !CardEffectHandle.Data.IsValid()) return;

	for (const FCardModifier& CardModifier : CardData->CardModifiers)
	{
		CardEffectHandle.Data->SetSetByCallerMagnitude(CardModifier.ValueTag, CardModifier.Value);
	}

	ApplyGameplayEffectSpecToTarget(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), CardEffectHandle, TargetDataHandle);
	
}
