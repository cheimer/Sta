// Sta Project - Multiplay RTS with GAS


#include "PlayWidget.h"

#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UPlayWidget::UpdateCost(float NewCost, float MaxCost)
{
	CostProgressBar->SetPercent(NewCost / MaxCost);
	
	FNumberFormattingOptions FormatOptions;
	FormatOptions.SetMaximumFractionalDigits(0);
	CostText->SetText(FText::AsNumber(NewCost, &FormatOptions));
}

void UPlayWidget::UpdateDrawCharge(float NewCharge, float MaxCharge)
{
	DrawProgressBar->SetPercent(NewCharge / MaxCharge);
}
