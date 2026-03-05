// Sta Project - Multiplay RTS with GAS

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "StaAIController.generated.h"

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

	/*
	void RequestMoveUnit(AAreaBase* SrcArea, AAreaBase* DestArea, float UnitNum);
	void RequestAttackArea(AAreaBase* SrcArea, AAreaBase* DestArea, float UnitNum);
	void RequestUseCard(const UCardData* CardData, AAreaBase* TargetArea);
	void RequestDrawCard();
	void RequestScanArea(AAreaBase* TargetArea);
	*/

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "Sta|AI")
	UBehaviorTree* BehaviorTree;

	UPROPERTY(EditDefaultsOnly, Category = "Sta|Deck")
	UAIStyleData* DeckData;

public:
	UAIStyleData* GetDeckData() const { return DeckData; }
	
};
