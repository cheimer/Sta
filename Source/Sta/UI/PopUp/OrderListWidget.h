// Sta Project - Multiplay RTS with GAS

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "PopUpWidget.h"
#include "Blueprint/UserWidget.h"
#include "OrderListWidget.generated.h"

class UInfoWidget;
class UOrderEntryWidget;
struct FInteractOption;
class UButton;
class UVerticalBox;

DECLARE_DELEGATE_OneParam(FOnOrderSelected, FGameplayTag SelectedTag);

/**
 * 
 */
UCLASS()
class STA_API UOrderListWidget : public UPopUpWidget
{
	GENERATED_BODY()

public:
	FOnOrderSelected OnOrderSelected;

protected:
	virtual void SetOptions(const TArray<FInteractOption>& Options) override;
	
	UPROPERTY(meta = (BindWidget))
	UVerticalBox* ButtonContainer;
	
	UPROPERTY(EditDefaultsOnly, Category = "Class")
	TSubclassOf<UOrderEntryWidget> OrderEntryClass;

	UPROPERTY(EditDefaultsOnly, Category = "Class")
	TSubclassOf<UInfoWidget> InfoWidgetClass;

private:
	void HandleOrderEntrySelected(FGameplayTag SelectedTag);

public:
	TSubclassOf<UInfoWidget> GetInfoWidgetClass() const { return InfoWidgetClass; }
	
};
