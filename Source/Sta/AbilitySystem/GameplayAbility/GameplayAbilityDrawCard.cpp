// Sta Project - Multiplay RTS with GAS


#include "GameplayAbilityDrawCard.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/AttributeSet/PlayerAttributeSet.h"
#include "Framework/GameMode/StaGameModeBase.h"
#include "GameFramework/PlayerState.h"
#include "GameplayTag/StaTags.h"
#include "Helper/StaHelper.h"

UGameplayAbilityDrawCard::UGameplayAbilityDrawCard()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	bRetriggerInstancedAbility = false;

	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::ServerOnly;
	
	FAbilityTriggerData TriggerData;
	TriggerData.TriggerTag = StaTags::Event::Card::Draw;
	TriggerData.TriggerSource = EGameplayAbilityTriggerSource::GameplayEvent;
	
	AbilityTriggers.Add(TriggerData);
}

void UGameplayAbilityDrawCard::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	UAbilitySystemComponent* ActorASC = GetAbilitySystemComponentFromActorInfo();
	AStaGameModeBase* GameMode = GetWorld()->GetAuthGameMode<AStaGameModeBase>();
	APlayerState* PS = Cast<APlayerState>(GetOwningActorFromActorInfo());

	if (!ActorASC || !GameMode || !PS || !ChargeResetEffect || !TriggerEventData->Target)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
		return;
	}

	APlayerController* PC = Cast<APlayerController>(PS->GetOwner());
    if (!PC)
    {
    	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
    	return;
    }
	
	if (!GameMode->CanDrawCard(PC))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
		return;
	}
	
	float CurrentCharge = ActorASC->GetNumericAttribute(UPlayerAttributeSet::GetChargeTimeAttribute());
	float MaxCharge = ActorASC->GetNumericAttribute(UPlayerAttributeSet::GetMaxChargeAttribute());
	
	bool bIsAutoDraw = FMath::IsNearlyEqual(CurrentCharge, MaxCharge);
	
	if (!bIsAutoDraw && !CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
		return;
	}
	
	ChargeTimeReset();

	GameMode->DrawCard(PC);
	
	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}

void UGameplayAbilityDrawCard::ChargeTimeReset()
{
	FGameplayEffectSpecHandle EffectSpec = MakeOutgoingGameplayEffectSpec(ChargeResetEffect);
	if (!EffectSpec.IsValid()) return;
	
	ApplyGameplayEffectSpecToOwner(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), EffectSpec);
}
