// Sta Project - Multiplay RTS with GAS

#pragma once

#include "CoreMinimal.h"
#include "Component/CardComponent.h"
#include "GameFramework/GameModeBase.h"
#include "StaGameModeBase.generated.h"

class AStaAIController;
class AAIPawn;
class UCardData;

USTRUCT()
struct FDeckState
{
	GENERATED_BODY()

public:
	UPROPERTY()
	TArray<TObjectPtr<const UCardData>> DeckCards;
	
	UPROPERTY()
	TArray<TObjectPtr<const UCardData>> DiscardCards;
	
	UPROPERTY()
	TArray<TObjectPtr<const UCardData>> HandCards;
	
};

/**
 * 
 */
UCLASS()
class STA_API AStaGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	AStaGameModeBase();
	
	virtual void PostLogin(APlayerController* NewPlayer) override;

	void InitDeckState(const TArray<FCardInfo>& DeckList, AController* Controller);
	
	bool CanDrawCard(const AController* Controller);
	bool CardInHand(const AController* Controller, const UCardData* CardData);

	void DrawCard(AController* Controller);
	void DiscardCard(AController* Controller, const UCardData* CardData);

protected:
	virtual void BeginPlay() override;

private:
	void ShuffleDeck(TArray<TObjectPtr<const UCardData>>& Cards);

	void StartGameSettings();
	
	void SpawnAIPawns();
	void InitAreaState();
	
	UPROPERTY()
	TMap<AController*, FDeckState> DeckStates;

	UPROPERTY(EditDefaultsOnly, Category = "Sta|Class")
	TSubclassOf<AStaAIController> AIControllerClass;

	UPROPERTY(EditDefaultsOnly, Category = "Sta|Class")
	TSubclassOf<AAIPawn> AIPawnClass;

	UPROPERTY(EditDefaultsOnly, Category = "Sta|Value")
	int32 PlayerNum = 2;

	UPROPERTY(EditDefaultsOnly, Category = "Sta|Value")
	int32 AINum = 2;

	UPROPERTY(EditDefaultsOnly, Category = "Sta|Value")
	int32 InitCardNum = 4;
	
	UPROPERTY(EditDefaultsOnly, Category = "Sta|Value")
	int32 MaxCardNum = 8;

	int32 CurrentTeamNum = 0;

};
