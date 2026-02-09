// Sta Project - Multiplay RTS with GAS

#pragma once

#include "CoreMinimal.h"
#include "PopUpWidget.h"
#include "InfoWidget.generated.h"

class UTextBlock;
class UButton;

DECLARE_DELEGATE(FOnCloseClicked);

/**
 * 
 */
UCLASS()
class STA_API UInfoWidget : public UPopUpWidget
{
	GENERATED_BODY()

public:
	FOnCloseClicked OnCloseClicked;

protected:
	virtual void NativeConstruct() override;

	virtual void SetOptions(const TArray<FInteractOption>& Options) override;

	UFUNCTION()
	void OnCloseButtonReleased();

	UPROPERTY(meta = (BindWidget))
	UButton* CloseButton;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* InfoText;

};
