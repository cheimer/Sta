// Sta Project - Multiplay RTS with GAS


#include "StaPlayerState.h"

#include "AbilitySystem/StaAbilitySystemComponent.h"
#include "AbilitySystem/StaAttributeSet.h"

AStaPlayerState::AStaPlayerState()
{
	SetNetUpdateFrequency(100.0f);
	
	AbilitySystemComponent = CreateDefaultSubobject<UStaAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	AttributeSet = CreateDefaultSubobject<UStaAttributeSet>(TEXT("AttributeSet"));
	
}

void AStaPlayerState::BeginPlay()
{ 
	Super::BeginPlay();

	if (HasAuthority())
	{
		GiveDefaultAbilities();		
	}
}

void AStaPlayerState::GiveDefaultAbilities()
{
	if (!AbilitySystemComponent || !HasAuthority()) return;

	for (const TSubclassOf<UGameplayAbility> AbilityClass : DefaultAbilities)
	{
		if (AbilityClass)
		{
			FGameplayAbilitySpec AbilitySpec(AbilityClass, 1);
			AbilitySystemComponent->GiveAbility(AbilitySpec);
		}
	}
}

UAbilitySystemComponent* AStaPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UAttributeSet* AStaPlayerState::GetAttributeSet() const
{
	return AttributeSet;
}
