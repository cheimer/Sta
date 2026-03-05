// Sta Project - Multiplay RTS with GAS

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "AIStyleData.generated.h"

struct FCardInfo;

/**
 * 
 */
UCLASS()
class STA_API UAIStyleData : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = "Deck")
	TArray<FCardInfo> DeckList;

	UPROPERTY(EditDefaultsOnly, Category = "Settings", meta = (ClampMax = 1.0f, ClampMin = 0.0f))
	float Aggressive = 0.5f;
	
	UPROPERTY(EditDefaultsOnly, Category = "Settings", meta = (ClampMax = 1.0f, ClampMin = 0.0f))
	float Careful = 0.5f;
	
	UPROPERTY(EditDefaultsOnly, Category = "Settings", meta = (ClampMax = 1.0f, ClampMin = 0.0f))
	float Tricky = 0.5f;
	
};
