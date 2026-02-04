// Sta Project - Multiplay RTS with GAS


#include "GameplayAbilityDiscardCard.h"

#include "DataAsset/CardData.h"
#include "Framework/GameMode/StaGameModeBase.h"
#include "GameFramework/PlayerState.h"
#include "GameplayTag/StaTags.h"
#include "Helper/StaHelper.h"

UGameplayAbilityDiscardCard::UGameplayAbilityDiscardCard()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	bRetriggerInstancedAbility = false;

	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
	
	FAbilityTriggerData TriggerData;
	TriggerData.TriggerTag = StaTags::Event::Card::Discard;
	TriggerData.TriggerSource = EGameplayAbilityTriggerSource::GameplayEvent;
	
	AbilityTriggers.Add(TriggerData);
}

void UGameplayAbilityDiscardCard::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                                  const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	AStaGameModeBase* GameMode = GetWorld()->GetAuthGameMode<AStaGameModeBase>();
	APlayerState* PS = Cast<APlayerState>(GetOwningActorFromActorInfo());

	if (!GameMode || !PS)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	APlayerController* PC = Cast<APlayerController>(PS->GetOwner());
	if (!PC)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	const UCardData* CardData = Cast<UCardData>(TriggerEventData->OptionalObject);
	if (!CardData)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}
	
	if (!GameMode->CardInHand(PC, CardData))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	GameMode->DiscardCard(PC, CardData);
	
	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}
