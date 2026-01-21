// Sta Project - Multiplay RTS with GAS


#include "StaHUD.h"

#include "AbilitySystemComponent.h"
#include "PlayWidget.h"
#include "AbilitySystem/AttributeSet/PlayerAttributeSet.h"
#include "Blueprint/UserWidget.h"
#include "Controller/StaPlayerController.h"
#include "GameplayTag/StaTags.h"

void AStaHUD::BeginPlay()
{
	Super::BeginPlay();

	CurrentWidget = CreateWidget<UPlayWidget>(GetWorld(), MainWidgetClass);
	CurrentWidget->AddToViewport();
	
}

void AStaHUD::SetASCBinding(UAbilitySystemComponent* OwnerASC)
{
	if (!OwnerASC) return;
	PlayerASC = OwnerASC;
	
	PlayerSet = PlayerASC->GetSet<UPlayerAttributeSet>();
	if (!PlayerSet) return;

	CostChangedHandle = PlayerASC->GetGameplayAttributeValueChangeDelegate(UPlayerAttributeSet::GetCostAttribute()).AddUObject(this, &ThisClass::OnCostChanged);
	DrawChargeChangedHandle = PlayerASC->GetGameplayAttributeValueChangeDelegate(UPlayerAttributeSet::GetChargeTimeAttribute()).AddUObject(this, &ThisClass::OnDrawChargeChanged);

	FOnAttributeChangeData CostStartUp;
	CostStartUp.Attribute = PlayerSet->GetCostAttribute();
	CostStartUp.OldValue = PlayerSet->GetCost();
	CostStartUp.NewValue = PlayerSet->GetCost();
	OnCostChanged(CostStartUp);
	
	FOnAttributeChangeData DrawChargeStartUp;
	DrawChargeStartUp.Attribute = PlayerSet->GetChargeTimeAttribute();
	DrawChargeStartUp.OldValue = PlayerSet->GetChargeTime();
	DrawChargeStartUp.NewValue = PlayerSet->GetChargeTime();
	OnDrawChargeChanged(DrawChargeStartUp);
	
}

void AStaHUD::OnDrawButtonClicked()
{
	AStaPlayerController* OwnerPlayerController = Cast<AStaPlayerController>(GetOwningPlayerController());
	if (!OwnerPlayerController) return;

	FGameplayEventData EventData;
	EventData.Target = OwnerPlayerController->GetPawn();
	
	OwnerPlayerController->ActiveGameplayEvent(StaTags::Event::Card::Draw, &EventData);
	
}

void AStaHUD::OnCostChanged(const FOnAttributeChangeData& ChangedData)
{
	if (!PlayerSet || !CurrentWidget) return;

	CurrentWidget->UpdateCost(ChangedData.NewValue, PlayerSet->GetMaxCost());
}

void AStaHUD::OnDrawChargeChanged(const FOnAttributeChangeData& ChangedData)
{
	if (!PlayerSet || !CurrentWidget) return;

	CurrentWidget->UpdateDrawCharge(ChangedData.NewValue, PlayerSet->GetMaxCharge());
}
