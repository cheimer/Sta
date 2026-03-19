// Sta Project - Multiplay RTS with GAS


#include "StaAIController.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "GameplayTagContainer.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Component/HTNComponent.h"


AStaAIController::AStaAIController()
{
	PrimaryActorTick.bCanEverTick = false;
	bWantsPlayerState = true;
	
	HTNComponent = CreateDefaultSubobject<UHTNComponent>("HTNComponent");
}

void AStaAIController::BeginPlay()
{
	Super::BeginPlay();

}

void AStaAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	
	check(BehaviorTree);
	
	RunBehaviorTree(BehaviorTree);
	if (GetBrainComponent())
	{
		GetBrainComponent()->SetComponentTickInterval(1.0f);
	}

}

void AStaAIController::RequestAction(FGameplayTag GameplayTag, const FGameplayEventData* EventData)
{
	if (!HasAuthority() || !GetPawn()) return;
	
	UAbilitySystemComponent* PawnASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn());
	if (!PawnASC) return;

	PawnASC->HandleGameplayEvent(GameplayTag, EventData);

}
