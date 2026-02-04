// Sta Project - Multiplay RTS with GAS

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OrderEntryWidget.generated.h"

class UButton;
class UTextBlock;
/**
 * 
 */
UCLASS()
class STA_API UOrderEntryWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetOrderText(const FText& InOrderText);
	void SetOrderButton(TFunction<void()> InReleaseCallback);

protected:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void HandleButtonReleased();
	
	UPROPERTY(meta = (BindWidget))
	UButton* OrderButton;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* OrderText;

private:
	TFunction<void()> OnReleaseCallback;
	
};
