// Sta Project - Multiplay RTS with GAS

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTag/StaTags.h"
#include "CardData.generated.h"

class UGameplayEffect;

USTRUCT(BlueprintType)
struct FCardModifier
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	FGameplayTag ValueTag = StaTags::SetByCaller::None;

	UPROPERTY(EditDefaultsOnly)
	float Value = 0.0f;
	
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
	FGameplayTag CardTag = StaTags::None;

	UPROPERTY(EditDefaultsOnly)
	float Cost = 0.0f;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayEffect> CardEffectClass = nullptr;

	UPROPERTY(EditDefaultsOnly)
	TArray<FCardModifier> CardModifiers;

};
