// Sta Project - Multiplay RTS with GAS


#include "NotifyListWidget.h"

#include "NotifyEntryWidget.h"
#include "Components/VerticalBox.h"

void UNotifyListWidget::AddNotify(ENotifyPriority NotifyPriority, const FText& NotifyText)
{
	if (!NotifyBox) return;

	UNotifyEntryWidget* NotifyEntryWidget = UNotifyEntryWidget::Create(GetWorld(), NotifyEntryClass, NotifyPriority, NotifyText, NotifyDuration);
	if (!NotifyEntryWidget) return;

	NotifyBox->AddChild(NotifyEntryWidget);
	if (NotifyBox->GetChildrenCount() > MaxEntryCount)
	{
		NotifyBox->GetChildAt(0)->RemoveFromParent();
	}
	
}
