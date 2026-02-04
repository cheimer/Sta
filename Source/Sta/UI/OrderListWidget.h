// Sta Project - Multiplay RTS with GAS

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Blueprint/UserWidget.h"
#include "OrderListWidget.generated.h"

class UOrderEntryWidget;
struct FInteractOption;
class UButton;
class UVerticalBox;

/**
 * 
 */
UCLASS()
class STA_API UOrderListWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	static UUserWidget* Create(const UWorld* WorldContextObject, TSubclassOf<UOrderListWidget> OrderWidgetClass,
		const TArray<FInteractOption>& Options, const FVector2D Location, const int32 Priority);

protected:
	virtual void NativeConstruct() override;

	void SetOptions(const UWorld* WorldContextObject, const TArray<FInteractOption>& Options);
	
	UPROPERTY(meta = (BindWidget))
	UVerticalBox* ButtonContainer;
	
	UPROPERTY(EditDefaultsOnly, Category = "Value")
	TSubclassOf<UOrderEntryWidget> OrderEntryClass;

private:
	void AreaCancel();
	void AreaInfo();
	void AreaMove();
	
	bool bIsCreatedFromFactory = false;
	
};
