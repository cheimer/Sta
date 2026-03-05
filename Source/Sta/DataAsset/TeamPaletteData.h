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
	UPROPERTY(EditDefaultsOnly)
	FLinearColor DefaultColor = FLinearColor::White;

	UPROPERTY(EditDefaultsOnly)
	TArray<FLinearColor> PaletteColor;

	FLinearColor GetColorByTeamId(const FGenericTeamId& TeamId) const
	{
		return PaletteColor.IsValidIndex(TeamId.GetId()) ? PaletteColor[TeamId.GetId()] : DefaultColor;
	}

	FLinearColor GetDefaultColor() const {return DefaultColor;}
	
};
