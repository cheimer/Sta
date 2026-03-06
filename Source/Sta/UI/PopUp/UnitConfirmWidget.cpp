// Sta Project - Multiplay RTS with GAS


#include "UnitConfirmWidget.h"

#include "Components/Button.h"
#include "Components/Slider.h"
#include "Components/TextBlock.h"
#include "GameFramework/PlayerState.h"

void UUnitConfirmWidget::NativeConstruct()
{
	Super::NativeConstruct();

	ConfirmButton->OnReleased.AddDynamic(this, &ThisClass::OnConfirmButtonReleased);
	CancelButton->OnReleased.AddDynamic(this, &ThisClass::OnCancelButtonReleased);
	UnitNumSlider->OnValueChanged.AddDynamic(this, &ThisClass::OnSliderValueChanged);
}

void UUnitConfirmWidget::SetOptions(const TArray<FInteractOption>& Options)
{
	Super::SetOptions(Options);

	if (Options.IsEmpty() || !Options[0].TargetActor.IsValid()) return;

	if (IInteractable* InteractActor = Cast<IInteractable>(Options[0].TargetActor.Get()))
	{
		FGenericTeamId OwningTeamId = FGenericTeamId::NoTeam;
		if (IGenericTeamAgentInterface* StateTeamID = Cast<IGenericTeamAgentInterface>(GetWorld()->GetFirstPlayerController()->GetPlayerState<APlayerState>()))
		{
			OwningTeamId = StateTeamID->GetGenericTeamId();
		}
	
		SrcInfoText->SetText(InteractActor->GetInfoText(OwningTeamId));
		SrcInfoText->SetColorAndOpacity(Options[0].DisplayColor);
	}

}

void UUnitConfirmWidget::SetUp(AActor* MoveTo, const float MaxUnitNum)
{
	if (IInteractable* InteractActor = Cast<IInteractable>(MoveTo))
	{
		FGenericTeamId OwningTeamId = FGenericTeamId::NoTeam;
		if (IGenericTeamAgentInterface* StateTeamID = Cast<IGenericTeamAgentInterface>(GetWorld()->GetFirstPlayerController()->GetPlayerState<APlayerState>()))
		{
			OwningTeamId = StateTeamID->GetGenericTeamId();
		}
	
		DestInfoText->SetText(InteractActor->GetInfoText(OwningTeamId));
		DestInfoText->SetColorAndOpacity(InteractActor->GetInteractOptions(OwningTeamId)[0].DisplayColor);
	}
	
	UnitNumSlider->SetMinValue(0.0f);
	UnitNumSlider->SetMaxValue(MaxUnitNum);
	UnitNumSlider->SetStepSize(1 / UnitNumSlider->GetMaxValue());
	
	MinNumText->SetText(FText::AsNumber(FMath::FloorToInt(0.0f)));
	MaxNumText->SetText(FText::AsNumber(FMath::FloorToInt(MaxUnitNum)));
	
	bIsSetUp = true;
}

void UUnitConfirmWidget::OnConfirmButtonReleased()
{
	check(bIsSetUp);
	
	OnConfirmClicked.ExecuteIfBound(FMath::FloorToInt(UnitNumSlider->GetValue()));
}

void UUnitConfirmWidget::OnCancelButtonReleased()
{
	check(bIsSetUp);
	
	OnCancelClicked.ExecuteIfBound();
}

void UUnitConfirmWidget::OnSliderValueChanged(const float NewValue)
{
	check(bIsSetUp);
	
	CurrentNumText->SetText(FText::AsNumber(FMath::FloorToInt(NewValue)));
}
