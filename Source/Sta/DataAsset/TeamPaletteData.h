// Sta Project - Multiplay RTS with GAS

#pragma once

#include "CoreMinimal.h"
#include "GenericTeamAgentInterface.h"
#include "Engine/DataAsset.h"
#include "TeamPaletteData.generated.h"

/**
 * 
 */
UCLASS()
class STA_API UTeamPaletteData : public UDataAsset
{
	GENERATED_BODY()
	
public:
	FLinearColor GetColorByTeamId(const FGenericTeamId& TeamId) const
	{
		return PaletteColor.IsValidIndex(TeamId.GetId()) ? PaletteColor[TeamId.GetId()] : DefaultColor;
	}

	FLinearColor GetDefaultColor() const {return DefaultColor;}

	float GetInvisibleValue() const {return InvisibleValue;}

	UPROPERTY(EditDefaultsOnly)
	TArray<FLinearColor> PaletteColor;

protected:
	UPROPERTY(EditDefaultsOnly)
	FLinearColor DefaultColor = FLinearColor::White;

	UPROPERTY(EditDefaultsOnly)
	float InvisibleValue = 0.3f;

};
