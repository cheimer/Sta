// Sta Project - Multiplay RTS with GAS


#include "OrderEntryWidget.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"

void UOrderEntryWidget::SetOrderText(const FText& InOrderText)
{
	OrderText->SetText(InOrderText);
	OrderText->SetVisibility(ESlateVisibility::HitTestInvisible);
}

void UOrderEntryWidget::SetOrderButton(TFunction<void()> InReleaseCallback)
{
	OnReleaseCallback = InReleaseCallback;
}

void UOrderEntryWidget::NativeConstruct()
{
	Super::NativeConstruct();

	OrderButton->OnReleased.AddDynamic(this, &ThisClass::HandleButtonReleased);
}

void UOrderEntryWidget::HandleButtonReleased()
{
	if (!OnReleaseCallback) return;

	OnReleaseCallback();
}
