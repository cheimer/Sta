// Sta Project - Multiplay RTS with GAS

#pragma once

#include "CoreMinimal.h"
#include "InputTriggers.h"
#include "GameFramework/PlayerController.h"
#include "StaPlayerController.generated.h"


class UInputMappingContext;

USTRUCT(BlueprintType)
struct FInputActionConfig
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> InputAction;
	
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	ETriggerEvent TriggerEvent = ETriggerEvent::Triggered;
	
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	FName FunctionName;
	
};


UCLASS()
class STA_API AStaPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AStaPlayerController();

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	virtual void Tick(float DeltaTime) override;

	virtual void OnPossess(APawn* InPawn) override;

	virtual void UpdateHoveredActor();

	UFUNCTION()
	void Interact(const FInputActionValue& Value);
	UFUNCTION()
	void Cancel(const FInputActionValue& Value);
	UFUNCTION()
	void Scroll(const FInputActionValue& Value);
	void EdgeScroll();

	UPROPERTY()
	TWeakObjectPtr<AActor> HoveredActor;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Sta|Input")
	UInputMappingContext* MappingContext;
	
	UPROPERTY(EditDefaultsOnly, Category = "Sta|Input")
	TArray<FInputActionConfig> InputActionConfigs;
	
	
};
