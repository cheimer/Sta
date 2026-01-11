// Sta Project - Multiplay RTS with GAS

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CardData.generated.h"

class UGameplayEffect;

UENUM()
enum class ECardType
{
	None,
	Spell,
	Employ
};

class UGameplayAbility;
/**
 * 
 */
UCLASS()
class STA_API UCardData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly)
	ECardType CardType = ECardType::None;

	UPROPERTY(EditDefaultsOnly)
	float Cost = 0.0f;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayEffect> CardEffect;
	
};
