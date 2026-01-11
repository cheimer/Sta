// Sta Project - Multiplay RTS with GAS


#include "StaHUD.h"

#include "Blueprint/UserWidget.h"

void AStaHUD::BeginPlay()
{
	Super::BeginPlay();

	CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), MainWidgetClass);
	CurrentWidget->AddToViewport();
	
}

void AStaHUD::OnDrawButtonClicked()
{
	
}
