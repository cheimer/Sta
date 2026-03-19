// Sta Project - Multiplay RTS with GAS

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayWidget.generated.h"

class UButton;
class UCommonNumericTextBlock;
class UTextBlock;
class UProgressBar;

DECLARE_MULTICAST_DELEGATE(FOnDrawClicked);
DECLARE_MULTICAST_DELEGATE(FOnEmployClicked);

/**
 * 
 */
UCLASS()
class STA_API UPlayWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	
	void SetButtonTeamColor(const FLinearColor NewTeamColor);
	void UpdateCost(float NewCost, float MaxCost);
	void UpdateDrawCharge(float NewCharge, float MaxCharge);

	FOnDrawClicked OnDrawClicked;
	FOnEmployClicked OnEmployClicked;

protected:
	UFUNCTION()
	void HandleDrawButtonHovered();
	UFUNCTION()
	void HandleDrawButtonUnhovered();
	UFUNCTION()
	void HandleDrawButtonPressed();
	UFUNCTION()
	void HandleDrawButtonClicked();
	
	UFUNCTION()
	void HandleEmployButtonClicked();
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UProgressBar* CostProgressBar;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* CostText;
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* DrawButton;
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UProgressBar* DrawProgressBar;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* EmployButton;

	FLinearColor TeamColor = FLinearColor::White;
	
};
