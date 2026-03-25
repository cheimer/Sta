// Sta Project - Multiplay RTS with GAS

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Subsystem/NotifySubsystem.h"
#include "NotifyListWidget.generated.h"

class UVerticalBox;
class UNotifyEntryWidget;
/**
 * 
 */
UCLASS()
class STA_API UNotifyListWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void AddNotify(ENotifyPriority NotifyPriority, const FText& NotifyText);

protected:
	UPROPERTY(meta = (BindWidget))
	UVerticalBox* NotifyBox;

	UPROPERTY(EditDefaultsOnly, Category = "Sta|Class")
	TSubclassOf<UNotifyEntryWidget> NotifyEntryClass;

	UPROPERTY(EditDefaultsOnly, Category = "Sta|Value")
	int32 MaxEntryCount = 5;

	UPROPERTY(EditDefaultsOnly, Category = "Sta|Value")
	float NotifyDuration = 3.0f;
	
};
