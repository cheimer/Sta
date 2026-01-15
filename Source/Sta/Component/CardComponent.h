// Sta Project - Multiplay RTS with GAS

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CardComponent.generated.h"

class ACardBase;

USTRUCT()
struct FCardInfo
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Sta|CardInfo")
	TSubclassOf<ACardBase> CardClass = nullptr;

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

	void CreateCard();
	void UseCard(ACardBase* Card, AActor* Target);

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "Sta|Value")
	TArray<FCardInfo> CardInfos;

	UPROPERTY(EditAnywhere, Category = "Sta|Value")
	FVector2D CardOffset = FVector2D(0.5f, 0.8f);
	
	UPROPERTY(EditAnywhere, Category = "Sta|Value")
	float CardHeight = 1000.0f;
	
	UPROPERTY(EditAnywhere, Category = "Sta|Value")
	int32 MaxCardNum = 8;
	
	UPROPERTY(EditAnywhere, Category = "Sta|Value")
	float CardSpace = 120.0f;

private:
	UPROPERTY()
	TArray<TObjectPtr<ACardBase>> HandCards;

	float DefaultArmLength = 0.0f;

};
