// Sta Project - Multiplay RTS with GAS


#include "StaHUD.h"

#include "AbilitySystemComponent.h"
#include "PlayWidget.h"
#include "AbilitySystem/AttributeSet/PlayerAttributeSet.h"
#include "Blueprint/UserWidget.h"
#include "Controller/StaPlayerController.h"
#include "GameplayTag/StaTags.h"
#include "Helper/StaHelper.h"
#include "PopUp/InfoWidget.h"
#include "PopUp/OrderListWidget.h"
#include "PopUp/PopUpWidget.h"

void AStaHUD::BeginPlay()
{
	Super::BeginPlay();

	CurrentWidget = CreateWidget<UPlayWidget>(GetWorld(), MainWidgetClass);
	CurrentWidget->AddToViewport();

	if (AStaPlayerController* PlayerController = Cast<AStaPlayerController>(GetOwningPlayerController()))
	{
		PlayerController->OnControllerStateChanged.AddUObject(this, &ThisClass::HandleControllerStateChanged);
		PlayerController->OnControllerCanceled.AddUObject(this, &ThisClass::HandleControllerCanceled);
	}
	
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
	
	OwnerPlayerController->TriggerGameplayEvent(StaTags::Event::Card::Draw, &EventData);
	
}

void AStaHUD::HandleControllerStateChanged(FGameplayTag BeforeState, FGameplayTag AfterState, const TArray<FInteractOption>& NewOptions)
{
	if (AfterState.MatchesTagExact(StaTags::State::Controller::Menu))
	{
		FVector2D WidgetLocation;
		APlayerController* OwningPC = GetOwningPlayerController();
		if (!OwningPC) return;

		OwningPC->GetMousePosition(WidgetLocation.X, WidgetLocation.Y);
		
		UPopUpWidget* NewPopUpWidget = UPopUpWidget::Create(GetWorld(), AreaOrderWidgetClass, NewOptions, WidgetLocation, MenuWidgets.Num() + 1);
		if (!NewPopUpWidget) return;

		if (UOrderListWidget* NewOrderWidget = Cast<UOrderListWidget>(NewPopUpWidget))
		{
			NewOrderWidget->OnOrderSelected.BindUObject(this, &ThisClass::HandleOrderSelected);
		}

		MenuWidgets.Add(NewPopUpWidget);
	}
	else if (BeforeState.MatchesTagExact(StaTags::State::Controller::Menu) && !AfterState.MatchesTagExact(StaTags::State::Controller::Menu))
	{
		for (UUserWidget* MenuWidget : MenuWidgets)
		{
			MenuWidget->RemoveFromParent();
		}
		MenuWidgets.Empty();
	}
}

void AStaHUD::HandleControllerCanceled()
{
	if (MenuWidgets.IsEmpty()) return;

	AStaPlayerController* PlayerController = Cast<AStaPlayerController>(GetOwningPlayerController());
	if (!PlayerController) return;
	
	UUserWidget* PopWidget = MenuWidgets.Pop();
	PopWidget->RemoveFromParent();
		
	if (MenuWidgets.IsEmpty())
	{
		PlayerController->SetControllerIdle();
	}
	else
	{
		MenuWidgets.Top()->SetVisibility(ESlateVisibility::Visible);
	}
}

void AStaHUD::HandleOrderSelected(FGameplayTag SelectedTag)
{
	AStaPlayerController* PlayerController = Cast<AStaPlayerController>(GetOwningPlayerController());
	if (!PlayerController) return;
	
	if (SelectedTag.MatchesTagExact(StaTags::Interaction::Area::Cancel))
	{
		UUserWidget* PopWidget = MenuWidgets.Pop();
		PopWidget->RemoveFromParent();
		
		if (MenuWidgets.IsEmpty())
		{
			PlayerController->SetControllerIdle();
		}
		else
		{
			MenuWidgets.Top()->SetVisibility(ESlateVisibility::Visible);
		}
	}
	else if (SelectedTag.MatchesTagExact(StaTags::Interaction::Area::Info))
	{
		if (UOrderListWidget* TopWidget = Cast<UOrderListWidget>(MenuWidgets.Top()))
		{
			FVector2D WidgetLocation;

			PlayerController->GetMousePosition(WidgetLocation.X, WidgetLocation.Y);
		
			UPopUpWidget* NewPopUpWidget = UPopUpWidget::Create(GetWorld(), TopWidget->GetInfoWidgetClass(),
			TopWidget->GetOptions(), WidgetLocation, MenuWidgets.Num() + 1);
			
			TopWidget->SetVisibility(ESlateVisibility::HitTestInvisible);
			MenuWidgets.Add(NewPopUpWidget);

			if (UInfoWidget* InfoWidget = Cast<UInfoWidget>(NewPopUpWidget))
			{
				InfoWidget->OnCloseClicked.BindUObject(this, &ThisClass::HandleControllerCanceled);
			}

		}
	}
	else if (SelectedTag.MatchesTagExact(StaTags::Interaction::Area::Move))
	{
		while (!MenuWidgets.IsEmpty())
		{
			UUserWidget* PopWidget = MenuWidgets.Pop();
			PopWidget->RemoveFromParent();
		}
		
		PlayerController->SetControllerTargeting();
		
	}
	else if (SelectedTag.MatchesTagExact(StaTags::Interaction::Area::Scan))
	{
		while (!MenuWidgets.IsEmpty())
		{
			UUserWidget* PopWidget = MenuWidgets.Pop();
			PopWidget->RemoveFromParent();
		}
		
		PlayerController->SetControllerIdle();
		PlayerController->ScanInteractingArea();
		
	}
	else
	{
		checkf(false, TEXT("%s : not defined tag"), *GetName());
	}
	
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
