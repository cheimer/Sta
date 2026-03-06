// Sta Project - Multiplay RTS with GAS

#pragma once

#include "CoreMinimal.h"
#include "PopUpWidget.h"
#include "UnitConfirmWidget.generated.h"

class UTextBlock;
class UButton;

DECLARE_DELEGATE_OneParam(FOnConfirmClicked, float MoveUnitNum);
DECLARE_DELEGATE(FOnCancelClicked);


class USlider;
/**
 * 
 */
UCLASS()
class STA_API UUnitConfirmWidget : public UPopUpWidget
{
	GENERATED_BODY()
	
public:
	void SetUp(AActor* MoveTo, const float MaxUnitNum);
	
	FOnConfirmClicked OnConfirmClicked;
	FOnCancelClicked OnCancelClicked;

protected:
	virtual void NativeConstruct() override;

	virtual void SetOptions(const TArray<FInteractOption>& Options) override;

	UFUNCTION()
	void OnConfirmButtonReleased();

	UFUNCTION()
	void OnCancelButtonReleased();

	UFUNCTION()
	void OnSliderValueChanged(const float NewValue);

	UPROPERTY(meta = (BindWidget))
	UTextBlock* SrcInfoText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* DestInfoText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* MinNumText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* MaxNumText;

	UPROPERTY(meta = (BindWidget))
	USlider* UnitNumSlider;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* CurrentNumText;

	UPROPERTY(meta = (BindWidget))
	UButton* ConfirmButton;

	UPROPERTY(meta = (BindWidget))
	UButton* CancelButton;

private:
	bool bIsSetUp = false;

};
