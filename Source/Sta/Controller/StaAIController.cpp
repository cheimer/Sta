// Sta Project - Multiplay RTS with GAS


#include "StaAIController.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "GameplayTagContainer.h"


AStaAIController::AStaAIController()
{
	PrimaryActorTick.bCanEverTick = false;
	bWantsPlayerState = true;
}

void AStaAIController::BeginPlay()
{
	Super::BeginPlay();

}

void AStaAIController::RequestAction(FGameplayTag GameplayTag, const FGameplayEventData* EventData)
{
	if (!HasAuthority() || !GetPawn()) return;
	
	UAbilitySystemComponent* PawnASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn());
	if (!PawnASC) return;

	PawnASC->HandleGameplayEvent(GameplayTag, EventData);

}
