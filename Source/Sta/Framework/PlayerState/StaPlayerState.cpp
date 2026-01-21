// Sta Project - Multiplay RTS with GAS


#include "StaPlayerState.h"

#include "AbilitySystem/StaAbilitySystemComponent.h"
#include "AbilitySystem/AttributeSet/PlayerAttributeSet.h"

AStaPlayerState::AStaPlayerState()
{
	SetNetUpdateFrequency(100.0f);
	
	AbilitySystemComponent = CreateDefaultSubobject<UStaAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	AttributeSet = CreateDefaultSubobject<UPlayerAttributeSet>(TEXT("AttributeSet"));

	
}

void AStaPlayerState::BeginPlay()
{ 
	Super::BeginPlay();

	if (HasAuthority())
	{
		GiveDefaultAbilities();
		ApplyDefaultEffects();
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

void AStaPlayerState::ApplyDefaultEffects()
{
	if (!AbilitySystemComponent || !HasAuthority()) return;

	for (const TSubclassOf<UGameplayEffect> EffectClass : DefaultEffects)
	{
		if (!EffectClass) continue;

		FGameplayEffectContextHandle ContextHandle = AbilitySystemComponent->MakeEffectContext();
		ContextHandle.AddSourceObject(this);
		
		FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(EffectClass, 1.0f , ContextHandle);

		if (!SpecHandle.IsValid()) continue;
		
		FActiveGameplayEffectHandle ActiveHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
		if (ActiveHandle.IsValid())
		{
			ActivatedEffectHandles.Add(ActiveHandle);
		}
	}

}

UAbilitySystemComponent* AStaPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UPlayerAttributeSet* AStaPlayerState::GetAttributeSet() const
{
	return AttributeSet;
}
