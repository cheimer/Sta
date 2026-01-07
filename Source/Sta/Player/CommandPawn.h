// Sta Project - Multiplay RTS with GAS

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "CommandPawn.generated.h"

class AMapInfo;
class UFloatingPawnMovement;
class USpringArmComponent;
class UCameraComponent;
class UCharacterMovementComponent;

UCLASS()
class STA_API ACommandPawn : public APawn
{
	GENERATED_BODY()

public:
	ACommandPawn();
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	virtual void Tick(float DeltaTime) override;

	void SetScrollHeight(const bool bIsUp);
	void MoveTo(FVector Direction);

protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleAnywhere, Category = "Components")
	USceneComponent* RootSceneComponent;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	USpringArmComponent* SpringArmComponent;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UCameraComponent* CameraComponent;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UFloatingPawnMovement* MovementComponent;

	UPROPERTY(EditAnywhere, Category = "Sta|Test")
	bool bIsUpperTeam = false;
	
private:
	UPROPERTY(EditAnywhere, Category = "Sta|Input")
	float ScrollSensitive = 20.0f;

	UPROPERTY(EditAnywhere, Category = "Sta|Input")
	float MapMoveSensitive = 100.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Sta|Camera")
	float TargetArmLengthGoTo = 1200.0f;
	
	UPROPERTY(EditDefaultsOnly, Category = "Sta|Camera")
	float TargetArmLengthMin = 800.0f;
	
	UPROPERTY(EditDefaultsOnly, Category = "Sta|Camera")
	float TargetArmLengthMax = 2000.0f;

	UPROPERTY()
	TObjectPtr<AMapInfo> MapInfo;

};
