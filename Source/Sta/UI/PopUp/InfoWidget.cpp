// Sta Project - Multiplay RTS with GAS


#include "InfoWidget.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "GameFramework/PlayerState.h"

void UInfoWidget::SetOptions(const TArray<FInteractOption>& Options)
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
		
		InfoText->SetText(InteractActor->GetInfoText(OwningTeamId));
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
