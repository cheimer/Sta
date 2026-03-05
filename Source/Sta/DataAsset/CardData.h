// Sta Project - Multiplay RTS with GAS

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTag/StaTags.h"
#include "CardData.generated.h"

class ACardBase;
class UCardTargeting;
class UGameplayEffect;
class UCardData;

USTRUCT(BlueprintType)
struct FCardModifier
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	FGameplayTag ValueTag = StaTags::SetByCaller::None;

	UPROPERTY(EditDefaultsOnly)
	float Value = 0.0f;
	
};

USTRUCT()
struct FCardInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Sta|CardInfo")
	TObjectPtr<UCardData> CardData = nullptr;

	UPROPERTY(EditAnywhere, Category = "Sta|CardInfo")
	int32 CardNum = 1;
	
};

/**
 * 
 */
UCLASS()
class STA_API UCardData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ACardBase> CardClass = nullptr;
	
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag CardTag = StaTags::None;

	UPROPERTY(EditDefaultsOnly)
	float Cost = 0.0f;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayEffect> CardEffectClass = nullptr;

	UPROPERTY(EditDefaultsOnly)
	TArray<FCardModifier> CardModifiers;

	UPROPERTY(EditDefaultsOnly, Instanced)
	TObjectPtr<UCardTargeting> CardTargeting = nullptr;

};
