// Sta Project - Multiplay RTS with GAS


#include "PlayWidget.h"

#include "Components/Button.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UPlayWidget::NativeConstruct()
{
	Super::NativeConstruct();

	DrawButton->OnHovered.AddDynamic(this, &ThisClass::HandleDrawButtonHovered);
	DrawButton->OnUnhovered.AddDynamic(this, &ThisClass::HandleDrawButtonUnhovered);
	DrawButton->OnPressed.AddDynamic(this, &ThisClass::HandleDrawButtonPressed);
	DrawButton->OnReleased.AddDynamic(this, &ThisClass::HandleDrawButtonClicked);
	EmployButton->OnReleased.AddDynamic(this, &ThisClass::HandleEmployButtonClicked);
}

void UPlayWidget::SetButtonTeamColor(const FLinearColor NewTeamColor)
{
	TeamColor = NewTeamColor;
	
	FButtonStyle DrawStyle= DrawButton->GetStyle();
	DrawStyle.Normal.TintColor = TeamColor;
	DrawStyle.Hovered.TintColor = TeamColor * 1.3f;
	DrawStyle.Pressed.TintColor = TeamColor * 0.7f;
	DrawButton->SetStyle(DrawStyle);
	DrawButton->SetColorAndOpacity(TeamColor);
	
	FButtonStyle EmployStyle= EmployButton->GetStyle();
	EmployStyle.Normal.TintColor = TeamColor;
	EmployStyle.Hovered.TintColor = TeamColor * 1.3f;
	EmployStyle.Pressed.TintColor = TeamColor * 0.7f;
	EmployButton->SetStyle(EmployStyle);
	EmployButton->SetColorAndOpacity(TeamColor);
}

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

void UPlayWidget::HandleDrawButtonHovered()
{
	DrawProgressBar->SetFillColorAndOpacity(TeamColor * 1.3f);
}

void UPlayWidget::HandleDrawButtonUnhovered()
{
    DrawProgressBar->SetFillColorAndOpacity(TeamColor);
}

void UPlayWidget::HandleDrawButtonPressed()
{
    DrawProgressBar->SetFillColorAndOpacity(TeamColor * 0.7f);
}

void UPlayWidget::HandleDrawButtonClicked()
{
    DrawProgressBar->SetFillColorAndOpacity(TeamColor);
	OnDrawClicked.Broadcast();
}

void UPlayWidget::HandleEmployButtonClicked()
{
	OnEmployClicked.Broadcast();
}
