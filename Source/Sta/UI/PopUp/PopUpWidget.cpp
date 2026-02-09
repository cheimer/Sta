// Sta Project - Multiplay RTS with GAS


#include "PopUpWidget.h"

#include "Interface/Interactable.h"

UPopUpWidget* UPopUpWidget::Create(const UWorld* WorldContextObject, TSubclassOf<UPopUpWidget> PopUpWidgetClass,
                                   const TArray<FInteractOption>& Options, const FVector2D Location, const int32 Priority)
{
	if (!PopUpWidgetClass) return nullptr;
	
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::ReturnNull);
	if (!World) return nullptr;
	
	UPopUpWidget* CreatedWidget = CreateWidget<UPopUpWidget>(World, PopUpWidgetClass);
	if (!CreatedWidget) return nullptr;

	CreatedWidget->SetPositionInViewport(Location);
	CreatedWidget->SetInputActionPriority(Priority);
	CreatedWidget->bIsCreatedFromFactory = true;

	CreatedWidget->AddToViewport();
	
	CreatedWidget->SetOptions(Options);

	return CreatedWidget;
}

void UPopUpWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	check(bIsCreatedFromFactory);
	
}
