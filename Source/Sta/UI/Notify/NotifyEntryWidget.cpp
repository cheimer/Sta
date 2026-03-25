// Sta Project - Multiplay RTS with GAS


#include "NotifyEntryWidget.h"

#include "Animation/WidgetAnimation.h"
#include "Components/TextBlock.h"

UNotifyEntryWidget* UNotifyEntryWidget::Create(const UWorld* WorldContextObject, TSubclassOf<UNotifyEntryWidget> EntryWidgetClass,
                                               ENotifyPriority NotifyPriority, const FText& Text, float Duration)
{
	if (!EntryWidgetClass) return nullptr;
	
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::ReturnNull);
	if (!World) return nullptr;
	
	UNotifyEntryWidget* CreatedWidget = CreateWidget<UNotifyEntryWidget>(World, EntryWidgetClass);
	if (!CreatedWidget) return nullptr;

	CreatedWidget->bIsCreatedFromFactory = true;
	CreatedWidget->AddToViewport();
	CreatedWidget->InitNotify(NotifyPriority, Text, Duration);
	
	return CreatedWidget;
}

void UNotifyEntryWidget::NativeConstruct()
{
	Super::NativeConstruct();

	check(bIsCreatedFromFactory);
	
}

void UNotifyEntryWidget::InitNotify(ENotifyPriority NotifyPriority, const FText& Text, float Duration)
{
	if (!GetWorld()) return;
	
	FSlateColor NotifyColor;
	switch (NotifyPriority)
	{
	case ENotifyPriority::Low:
		NotifyColor = FColor::Green;
		break;
	case ENotifyPriority::Medium:
		NotifyColor = FColor::Yellow;
		break;
	case ENotifyPriority::High:
		NotifyColor = FColor::Red;
		break;
	default:
		check(false);
	}
	
	NotifyText->SetText(Text);
	NotifyText->SetColorAndOpacity(NotifyColor);
	
	if (FadeOutAnim)
	{
		FWidgetAnimationDynamicEvent FinishDelegate;
		FinishDelegate.BindDynamic(this, &ThisClass::OnFadeOutFinish);
		BindToAnimationFinished(FadeOutAnim, FinishDelegate);
	}
	
	GetWorld()->GetTimerManager().SetTimer(DurationHandle, this, &ThisClass::NotifyDurationFinish, Duration, false);

}

void UNotifyEntryWidget::NotifyDurationFinish()
{
	if (FadeOutAnim)
	{
		SetVisibility(ESlateVisibility::HitTestInvisible);
		PlayAnimation(FadeOutAnim);
	}
	else
	{
		RemoveFromParent();
	}
}

void UNotifyEntryWidget::OnFadeOutFinish()
{
	RemoveFromParent();
}
