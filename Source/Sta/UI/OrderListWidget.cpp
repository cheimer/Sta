// Sta Project - Multiplay RTS with GAS


#include "OrderListWidget.h"

#include "OrderEntryWidget.h"
#include "Components/VerticalBox.h"
#include "GameplayTag/StaTags.h"
#include "Helper/StaHelper.h"
#include "Interface/Interactable.h"

UUserWidget* UOrderListWidget::Create(const UWorld* WorldContextObject, TSubclassOf<UOrderListWidget> OrderWidgetClass,
                                      const TArray<FInteractOption>& Options, const FVector2D Location, const int32 Priority)
{
	if (!OrderWidgetClass) return nullptr;
	
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::ReturnNull);
	if (!World) return nullptr;
	
	UOrderListWidget* CreatedWidget = CreateWidget<UOrderListWidget>(World, OrderWidgetClass);
	if (!CreatedWidget) return nullptr;

	CreatedWidget->SetPositionInViewport(Location);
	CreatedWidget->SetInputActionPriority(Priority);
	CreatedWidget->bIsCreatedFromFactory = true;

	CreatedWidget->AddToViewport();
	CreatedWidget->SetOptions(WorldContextObject, Options);

	return CreatedWidget;
}

void UOrderListWidget::SetOptions(const UWorld* WorldContextObject, const TArray<FInteractOption>& Options)
{
	if (!OrderEntryClass) return;
	
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::ReturnNull);
	if (!World) return;
	
	for (const FInteractOption& Option : Options)
	{
		UOrderEntryWidget* OptionEntryWidget = CreateWidget<UOrderEntryWidget>(World, OrderEntryClass);
		if (!OptionEntryWidget) continue;;
		
		OptionEntryWidget->SetOrderText(Option.DisplayName);

		if (Option.InteractTag.MatchesTagExact(StaTags::Interaction::Area::Cancel))
		{
			OptionEntryWidget->SetOrderButton([this](){AreaCancel();});
		}
		else if (Option.InteractTag.MatchesTagExact(StaTags::Interaction::Area::Info))
		{
			OptionEntryWidget->SetOrderButton([this](){AreaInfo();});
		}
		else if (Option.InteractTag.MatchesTagExact(StaTags::Interaction::Area::Move))
		{
			OptionEntryWidget->SetOrderButton([this](){AreaMove();});
		}
		else
		{
			checkf(false, TEXT("%s : not defined tag"), *GetName());
			break;
		}
		

		ButtonContainer->AddChildToVerticalBox(OptionEntryWidget);
	}
}

void UOrderListWidget::NativeConstruct()
{
	Super::NativeConstruct();

	check(bIsCreatedFromFactory);
	
}

void UOrderListWidget::AreaCancel()
{
	StaDebug::Print(TEXT("Cancel"));
	Own
}

void UOrderListWidget::AreaInfo()
{
	StaDebug::Print(TEXT("Info"));
}

void UOrderListWidget::AreaMove()
{
	StaDebug::Print(TEXT("Move"));
}
