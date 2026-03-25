// Sta Project - Multiplay RTS with GAS

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Interface/Interactable.h"
#include "Subsystem/NotifySubsystem.h"
#include "StaHUD.generated.h"

class UNotifyListWidget;
class UNotifyEntryWidget;
class UPopUpWidget;
struct FGameplayTag;
class UOrderListWidget;
class UPlayerAttributeSet;
class UPlayWidget;
struct FOnAttributeChangeData;
class UAbilitySystemComponent;

/**
 * 
 */
UCLASS()
class STA_API AStaHUD : public AHUD
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

	virtual void SetASCBinding(UAbilitySystemComponent* OwnerASC);

	UFUNCTION(BlueprintCallable, Category = "Sta|WidgetFunc")
	void OnDrawButtonClicked();

	UFUNCTION(BlueprintCallable, Category = "Sta|WidgetFunc")
	void OnEmployeeButtonClicked();

	void HandleControllerStateChanged(FGameplayTag BeforeState, FGameplayTag AfterState, const TArray<FInteractOption>& NewOptions);
	void HandleControllerCanceled();

	void SetTeamColor(const FLinearColor NewTeamColor);

	void NotifyText(ENotifyPriority NotifyPriority, const FText& NotifyText);

protected:
	void OnCostChanged(const FOnAttributeChangeData& ChangedData);
	void OnDrawChargeChanged(const FOnAttributeChangeData& ChangedData);

	void HandleOrderSelected(FGameplayTag SelectedTag);
	
	UPROPERTY(EditDefaultsOnly, Category = "Sta|Widget")
	TSubclassOf<UPlayWidget> MainWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "Sta|Widget")
	TSubclassOf<UPopUpWidget> AreaOrderWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "Sta|Widget")
	TSubclassOf<UPopUpWidget> AreaUnitConfirmWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "Sta|Widget")
	TSubclassOf<UNotifyListWidget> NotifyListWidgetClass;

	UPROPERTY(Transient)
	UPlayWidget* CurrentWidget;

	UPROPERTY(Transient)
	UNotifyListWidget* NotifyWidget;

private:
	FDelegateHandle CostChangedHandle;
	FDelegateHandle DrawChargeChangedHandle;

	UPROPERTY()
	TWeakObjectPtr<UAbilitySystemComponent> PlayerASC;

	UPROPERTY()
	const UPlayerAttributeSet* PlayerSet;

	UPROPERTY()
	TArray<UUserWidget*> MenuWidgets;
	
	FLinearColor TeamColor = FLinearColor::White;

public:
	UFUNCTION(BlueprintCallable, Category = "Sta|WidgetFunc")
	FLinearColor GetTeamColor() const {return TeamColor;}
	
};
