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
class STA_API UCardComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UCardComponent();

	void CreateCard();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "Sta|Value")
	TArray<FCardInfo> Cards;

};
