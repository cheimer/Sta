// Sta Project - Multiplay RTS with GAS

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "StaHUD.generated.h"

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

protected:
	void OnCostChanged(const FOnAttributeChangeData& ChangedData);
	void OnDrawChargeChanged(const FOnAttributeChangeData& ChangedData);
	
	UPROPERTY(EditDefaultsOnly, Category = "Sta|Widget")
	TSubclassOf<UPlayWidget> MainWidgetClass;

	UPROPERTY()
	UPlayWidget* CurrentWidget;

private:
	FDelegateHandle CostChangedHandle;
	FDelegateHandle DrawChargeChangedHandle;

	UPROPERTY()
	TWeakObjectPtr<UAbilitySystemComponent> PlayerASC;

	UPROPERTY()
	const UPlayerAttributeSet* PlayerSet;
	
};
