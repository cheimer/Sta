// Sta Project - Multiplay RTS with GAS

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CardComponent.generated.h"

class ADiscardCard;
class UCardData;
class ACardBase;

USTRUCT()
struct FCardInfo
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Sta|CardInfo")
	TObjectPtr<UCardData> CardData = nullptr;

	UPROPERTY(EditAnywhere, Category = "Sta|CardInfo")
	int32 CardNum = 1;
	
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class STA_API UCardComponent : public USceneComponent
{
	GENERATED_BODY()

public:
	UCardComponent();

	void UpdateCardOffset();

	void AddCardToHand(const UCardData* DrawCardData);
	void RemoveCardFromHand(const UCardData* RemoveCardData);
	
	void UseCard(ACardBase* Card, AActor* Target);
	void DiscardCard(ACardBase* Card);

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "Sta|CardInfo")
	TArray<FCardInfo> DeckList;

	UPROPERTY(EditAnywhere, Category = "Sta|Value")
	FVector2D CardOffset = FVector2D(0.5f, 0.8f);
	
	UPROPERTY(EditAnywhere, Category = "Sta|Value")
	float CardHeight = 1000.0f;
	
	UPROPERTY(EditAnywhere, Category = "Sta|Value")
	int32 MaxCardNum = 8;
	
	UPROPERTY(EditAnywhere, Category = "Sta|Value")
	float CardSpace = 120.0f;

	UPROPERTY(EditAnywhere, Category = "Sta|Discard")
	FVector2D DiscardDistance = FVector2D(-200.0f, -800.0f);
	
private:
	void CreateDiscardCard();
	
	UPROPERTY(EditDefaultsOnly, Category = "Sta|Discard")
	TSubclassOf<ADiscardCard> DiscardClass;
	
	UPROPERTY(Transient)
	TObjectPtr<ADiscardCard> DiscardCardActor;

	UPROPERTY()
	TArray<TObjectPtr<ACardBase>> HandCards;

	float DefaultArmLength = 0.0f;

	UPROPERTY(Transient)
	TObjectPtr<ACardBase> RecentCard;

public:
	const TArray<FCardInfo>& GetDeckList() { return DeckList; }
	

};
