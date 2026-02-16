// Sta Project - Multiplay RTS with GAS

#pragma once

#include "CoreMinimal.h"
#include "Component/CardComponent.h"
#include "GameFramework/GameModeBase.h"
#include "StaGameModeBase.generated.h"

class UCardData;

USTRUCT()
struct FPlayerDeckState
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

	void InitDeckState(const TArray<FCardInfo>& DeckList, APlayerController* PC);
	
	bool CanDrawCard(APlayerController* PC);
	bool CardInHand(APlayerController* PC, const UCardData* CardData);

	void DrawCard(APlayerController* PC);
	void DiscardCard(APlayerController* PC, const UCardData* CardData);

protected:
	virtual void BeginPlay() override;

private:
	void ShuffleDeck(TArray<TObjectPtr<const UCardData>>& Cards);
	
	UPROPERTY()
	TMap<APlayerController*, FPlayerDeckState> PlayerDeckState;

	UPROPERTY(EditDefaultsOnly, Category = "Sta|Value")
	int32 InitCardNum = 4;
	
	UPROPERTY(EditDefaultsOnly, Category = "Sta|Value")
	int32 MaxCardNum = 8;

	int32 CurrentTeamNum = 0;

};
