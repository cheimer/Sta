// Sta Project - Multiplay RTS with GAS


#include "InfoWidget.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"

void UInfoWidget::SetOptions(const TArray<FInteractOption>& Options)
{
	Super::SetOptions(Options);

	if (Options.IsEmpty() || !Options[0].TargetActor.IsValid()) return;
	
	if (IInteractable* InteractActor = Cast<IInteractable>(Options[0].TargetActor.Get()))
	{
		InfoText->SetText(InteractActor->GetInfoText());
	}
}

void UInfoWidget::NativeConstruct()
{
	Super::NativeConstruct();

	CloseButton->OnReleased.AddDynamic(this, &ThisClass::OnCloseButtonReleased);
	
}

void UInfoWidget::OnCloseButtonReleased()
{
	OnCloseClicked.ExecuteIfBound();
}
