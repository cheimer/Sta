// Sta Project - Multiplay RTS with GAS

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Interface/Interactable.h"
#include "StaHUD.generated.h"

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

	UPROPERTY(Transient)
	UPlayWidget* CurrentWidget;

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
