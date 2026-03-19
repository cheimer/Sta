// Sta Project - Multiplay RTS with GAS


#include "StaPlayerState.h"

#include "AbilitySystem/StaAbilitySystemComponent.h"
#include "AbilitySystem/AttributeSet/PlayerAttributeSet.h"
#include "GameplayTag/StaTags.h"
#include "Helper/StaHelper.h"
#include "Net/UnrealNetwork.h"

AStaPlayerState::AStaPlayerState()
{
	SetNetUpdateFrequency(100.0f);
	
	AbilitySystemComponent = CreateDefaultSubobject<UStaAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	AttributeSet = CreateDefaultSubobject<UPlayerAttributeSet>(TEXT("AttributeSet"));

	
}

void AStaPlayerState::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, PlayerTeamID);
}

void AStaPlayerState::BeginPlay()
{ 
	Super::BeginPlay();

	if (HasAuthority())
	{
		GiveDefaultAbilities();
		ApplyDefaultEffects();
	}

	if (HasAuthority())
	{
		if (GetAttributeSet())
		{
			GetAttributeSet()->OnChargeComplete.AddUObject(this, &ThisClass::HandleChargeComplete);
		}
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

void AStaPlayerState::HandleChargeComplete()
{
	AController* OwningController = Cast<AController>(GetOwner());
	if (!GetAbilitySystemComponent() || !OwningController || !OwningController->GetPawn()) return;

	float CurrentTime = GetWorld()->GetTimeSeconds();
	if(CurrentTime - RecentChargeDrawTime < 0.1f)
	{
		return;
	}
	
	RecentChargeDrawTime = CurrentTime;
	
	FGameplayEventData EventData;
	EventData.Target = OwningController->GetPawn();
	
	GetAbilitySystemComponent()->HandleGameplayEvent(StaTags::Event::Card::Draw, &EventData);
}

UAbilitySystemComponent* AStaPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UPlayerAttributeSet* AStaPlayerState::GetAttributeSet() const
{
	return AttributeSet;
}

/**
 * GenericTeamAgentInterface
 */
void AStaPlayerState::SetGenericTeamId(const FGenericTeamId& TeamID)
{
	PlayerTeamID = TeamID;
}

FGenericTeamId AStaPlayerState::GetGenericTeamId() const
{
	return PlayerTeamID;
}
