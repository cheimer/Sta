// Sta Project - Multiplay RTS with GAS

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayWidget.generated.h"

class UCommonNumericTextBlock;
class UTextBlock;
class UProgressBar;
/**
 * 
 */
UCLASS()
class STA_API UPlayWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void UpdateCost(float NewCost, float MaxCost);
	void UpdateDrawCharge(float NewCharge, float MaxCharge);

protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UProgressBar* CostProgressBar;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UCommonNumericTextBlock* CostNumericText;
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UProgressBar* DrawProgressBar;

};
