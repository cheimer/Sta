// Sta Project - Multiplay RTS with GAS

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Interface/Interactable.h"
#include "PopUpWidget.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class STA_API UPopUpWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	static UPopUpWidget* Create(const UWorld* WorldContextObject, TSubclassOf<UPopUpWidget> PopUpWidgetClass,
		const TArray<FInteractOption>& Options, const FVector2D Location, const int32 Priority);

	const TArray<FInteractOption>& GetOptions() { return InteractOptions; }

protected:
	virtual void NativeConstruct() override;

	virtual void SetOptions(const TArray<FInteractOption>& Options) { InteractOptions = Options; }

	TArray<FInteractOption> InteractOptions;

private:
	bool bIsCreatedFromFactory = false;
	
};
