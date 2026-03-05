// Sta Project - Multiplay RTS with GAS


#include "AIPawn.h"

#include "AbilitySystemComponent.h"
#include "GameFramework/PlayerState.h"


AAIPawn::AAIPawn()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = false;
	AutoPossessAI = EAutoPossessAI::Disabled;
}

void AAIPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

void AAIPawn::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (!IsValid(GetAbilitySystemComponent()) || !IsValid(GetPlayerState())) return;

	if (HasAuthority())
	{
		GetAbilitySystemComponent()->InitAbilityActorInfo(GetPlayerState(), this);
	}
}

UAbilitySystemComponent* AAIPawn::GetAbilitySystemComponent() const
{
	IAbilitySystemInterface* AbilitySystemInterface = Cast<IAbilitySystemInterface>(GetPlayerState());
	if (!AbilitySystemInterface) return nullptr;
	
	return AbilitySystemInterface->GetAbilitySystemComponent();
}
