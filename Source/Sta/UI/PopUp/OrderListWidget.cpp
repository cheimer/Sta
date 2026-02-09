// Sta Project - Multiplay RTS with GAS


#include "OrderListWidget.h"

#include "OrderEntryWidget.h"
#include "Components/VerticalBox.h"
#include "Interface/Interactable.h"

void UOrderListWidget::SetOptions(const TArray<FInteractOption>& Options)
{
	Super::SetOptions(Options);
	
	if (!OrderEntryClass) return;
	if (!GetWorld()) return;
	
	for (const FInteractOption& Option : Options)
	{
		UOrderEntryWidget* OptionEntryWidget = CreateWidget<UOrderEntryWidget>(GetWorld(), OrderEntryClass);
		if (!OptionEntryWidget) continue;;
		
		OptionEntryWidget->SetOrderText(Option.DisplayName);
		OptionEntryWidget->SetOrderButton([this, Option]()
		{
			HandleOrderEntrySelected(Option.InteractTag);
		});

		ButtonContainer->AddChildToVerticalBox(OptionEntryWidget);
	}
}

void UOrderListWidget::HandleOrderEntrySelected(FGameplayTag SelectedTag)
{
	OnOrderSelected.ExecuteIfBound(SelectedTag);
}
