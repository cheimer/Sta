// Sta Project - Multiplay RTS with GAS


#include "CommandPawn.h"

#include "AbilitySystemComponent.h"
#include "Camera/CameraComponent.h"
#include "Component/CardComponent.h"
#include "Framework/MapInfo.h"
#include "Framework/PlayerState/StaPlayerState.h"
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

	CardComponent = CreateDefaultSubobject<UCardComponent>("CardComponent");
	CardComponent->SetupAttachment(RootComponent);
	
}

void ACommandPawn::BeginPlay()
{
	Super::BeginPlay();

	MapInfo = AMapInfo::GetInstance(GetWorld());

	if (bIsUpperTeam)
	{
		SpringArmComponent->SetRelativeRotation(FRotator(-70.0f, 180.0f, 0.0f));
	}

	//TODO: Test Card. Remove This later
	if (IsLocallyControlled())
	{
		CardComponent->CreateCard();
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
	
	CardComponent->UpdateCardOffset();
}

void ACommandPawn::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	
	if (!IsValid(GetAbilitySystemComponent()) || !IsValid(GetPlayerState())) return;

	if (HasAuthority())
	{
		GetAbilitySystemComponent()->InitAbilityActorInfo(GetPlayerState(), this);
	}
	
}

void ACommandPawn::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	if (!IsValid(GetAbilitySystemComponent()) || !IsValid(GetPlayerState())) return;

	GetAbilitySystemComponent()->InitAbilityActorInfo(GetPlayerState(), this);
	
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
		FVector AdjustLocation(AdjustLocation2D, GetActorLocation().Z);
		SetActorLocation(AdjustLocation);
		return;
		
	}
	
	AddMovementInput(Direction, MapMoveSensitive);
}

UAbilitySystemComponent* ACommandPawn::GetAbilitySystemComponent() const
{
	AStaPlayerState* StaPlayerState = Cast<AStaPlayerState>(GetPlayerState());
	if (!StaPlayerState) return nullptr;
	
	return StaPlayerState->GetAbilitySystemComponent();
}

UAttributeSet* ACommandPawn::GetAttributeSet() const
{
	AStaPlayerState* StaPlayerState = Cast<AStaPlayerState>(GetPlayerState());
	if (!StaPlayerState) return nullptr;
	
	return StaPlayerState->GetAttributeSet();
}

void ACommandPawn::SetScrollHeight(const bool bIsUp)
{
	float ScrollAmount = bIsUp ? TargetArmLengthGoTo + ScrollSensitive : TargetArmLengthGoTo - ScrollSensitive;
	
	TargetArmLengthGoTo = FMath::Clamp(ScrollAmount, TargetArmLengthMin, TargetArmLengthMax);
}

float ACommandPawn::GetTargetArmLength()
{
	if (!SpringArmComponent) return 0.0f;

	return SpringArmComponent->TargetArmLength;
}
