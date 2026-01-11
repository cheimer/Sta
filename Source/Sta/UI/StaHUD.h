// Sta Project - Multiplay RTS with GAS

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "StaHUD.generated.h"

/**
 * 
 */
UCLASS()
class STA_API AStaHUD : public AHUD
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
	
	UFUNCTION(BlueprintCallable, Category = "Sta|WidgetFunc")
	void OnDrawButtonClicked();

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Sta|Widget")
	TSubclassOf<UUserWidget> MainWidgetClass;

	UPROPERTY()
	UUserWidget* CurrentWidget;
	
};
