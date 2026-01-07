// Sta Project - Multiplay RTS with GAS


#include "CommandPawn.h"

#include "Camera/CameraComponent.h"
#include "Framework/MapInfo.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "GameFramework/SpringArmComponent.h"


ACommandPawn::ACommandPawn()
{
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	RootSceneComponent = CreateDefaultSubobject<USceneComponent>("RootSceneComponent");
	SetRootComponent(RootSceneComponent);
	
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
	SpringArmComponent->SetupAttachment(RootComponent);
	SpringArmComponent->TargetArmLength = TargetArmLengthGoTo;
	SpringArmComponent->SetRelativeRotation(FRotator(-70.0f, 0.0f, 0.0f));
	SpringArmComponent->bUsePawnControlRotation = false;
	SpringArmComponent->bInheritPitch = false;
	SpringArmComponent->bInheritYaw = false;
	SpringArmComponent->bInheritRoll = false;
	SpringArmComponent->bDoCollisionTest = false;
	SpringArmComponent->bEnableCameraLag = true;
	
	CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	CameraComponent->SetupAttachment(SpringArmComponent);
	CameraComponent->bUsePawnControlRotation = false;

	MovementComponent = CreateDefaultSubobject<UFloatingPawnMovement>("MovementComponent");
	MovementComponent->MaxSpeed = 3000.0f;
	MovementComponent->Acceleration = 8000.0f;
	MovementComponent->Deceleration = 8000.0f;
	MovementComponent->SetIsReplicated(true);
	
}

void ACommandPawn::BeginPlay()
{
	Super::BeginPlay();

	MapInfo = AMapInfo::GetInstance(GetWorld());

	if (bIsUpperTeam)
	{
		SpringArmComponent->SetRelativeRotation(FRotator(-70.0f, 180.0f, 0.0f));
	}
}

void ACommandPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ACommandPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (!FMath::IsNearlyEqual(SpringArmComponent->TargetArmLength, TargetArmLengthGoTo))
	{
		SpringArmComponent->TargetArmLength = FMath::Lerp(SpringArmComponent->TargetArmLength, TargetArmLengthGoTo, DeltaTime * 10.0f);
	}
	
}

void ACommandPawn::SetScrollHeight(const bool bIsUp)
{
	float ScrollAmount = bIsUp ? TargetArmLengthGoTo + ScrollSensitive : TargetArmLengthGoTo - ScrollSensitive;
	
	TargetArmLengthGoTo = FMath::Clamp(ScrollAmount, TargetArmLengthMin, TargetArmLengthMax);
}

void ACommandPawn::MoveTo(FVector Direction)
{
	if (bIsUpperTeam)
	{
		Direction *= -1.0f;
	}
	
	FVector2D NextLocation = FVector2D(GetActorLocation() + Direction * MapMoveSensitive);
	if (MapInfo->IsOutMap(NextLocation))
	{
		FVector2D AdjustLocation2D = MapInfo->ClampLocation(FVector2D(GetActorLocation()));
		FVector AdjustLocation = FVector(AdjustLocation2D.X, AdjustLocation2D.Y, GetActorLocation().Z);
		SetActorLocation(AdjustLocation);
		return;
		
	}
	
	AddMovementInput(Direction, MapMoveSensitive);
}
