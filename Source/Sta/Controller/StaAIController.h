// Sta Project - Multiplay RTS with GAS

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "StaAIController.generated.h"

class UHTNComponent;
class UAIStyleData;
class UCardData;
class AAreaBase;
struct FGameplayEventData;
struct FGameplayTag;

UCLASS()
class STA_API AStaAIController : public AAIController
{
	GENERATED_BODY()

public:
	AStaAIController();

	void RequestAction(FGameplayTag GameplayTag, const FGameplayEventData* EventData);

protected:
	virtual void BeginPlay() override;

	virtual void OnPossess(APawn* InPawn) override;

	UPROPERTY(EditDefaultsOnly, Category = "Sta|AI")
	UBehaviorTree* BehaviorTree;

	UPROPERTY(EditDefaultsOnly, Category = "Sta|Deck")
	UAIStyleData* DeckData;
	
	UPROPERTY(EditDefaultsOnly, Category = "Component")
	UHTNComponent* HTNComponent;


public:
	UAIStyleData* GetDeckData() const { return DeckData; }
	
};
