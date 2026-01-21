// Sta Project - Multiplay RTS with GAS


#include "PlayWidget.h"

#include "CommonNumericTextBlock.h"
#include "Components/ProgressBar.h"

void UPlayWidget::UpdateCost(float NewCost, float MaxCost)
{
	CostProgressBar->SetPercent(NewCost / MaxCost);
	CostNumericText->SetCurrentValue(NewCost);
}

void UPlayWidget::UpdateDrawCharge(float NewCharge, float MaxCharge)
{
	DrawProgressBar->SetPercent(NewCharge / MaxCharge);
}
