// Sta Project - Multiplay RTS with GAS

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Subsystem/NotifySubsystem.h"
#include "NotifyEntryWidget.generated.h"

class UTextBlock;
/**
 * 
 */
UCLASS()
class STA_API UNotifyEntryWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	static UNotifyEntryWidget* Create(const UWorld* WorldContextObject, TSubclassOf<UNotifyEntryWidget> EntryWidgetClass,
		ENotifyPriority NotifyPriority, const FText& Text, float Duration);

protected:
	virtual void NativeConstruct() override;
	
	void InitNotify(ENotifyPriority NotifyPriority, const FText& Text, float Duration);
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* NotifyText;

	UPROPERTY(Transient, meta = (BindWidgetAnimOptional))
	UWidgetAnimation* FadeOutAnim;

private:
	UFUNCTION()
	void NotifyDurationFinish();
	UFUNCTION()
	void OnFadeOutFinish();

	FTimerHandle DurationHandle;
	
	bool bIsCreatedFromFactory = false;
	
};
