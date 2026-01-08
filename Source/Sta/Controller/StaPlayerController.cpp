// Sta Project - Multiplay RTS with GAS


#include "StaPlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Helper/StaHelper.h"
#include "Interface/Interactable.h"
#include "Player/CommandPawn.h"
#include "Type/StaEnum.h"

AStaPlayerController::AStaPlayerController()
{
	
	
}

void AStaPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	if(UEnhancedInputLocalPlayerSubsystem* EnhancedInputSubsystem =
				ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		EnhancedInputSubsystem->AddMappingContext(MappingContext, static_cast<int32>(EInputPriority::Controller));
	}
	
	bShowMouseCursor = true;

	FInputModeGameAndUI CurrentInputMode;
	CurrentInputMode.SetLockMouseToViewportBehavior(EMouseLockMode::LockAlways);
	SetInputMode(CurrentInputMode);
	
}

void AStaPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if(UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		for (const auto& [InputActionPtr, TriggerEvent, FunctionName] : InputActionConfigs)
		{
			if (!FindFunction(FunctionName))
			{
				UE_LOG(LogTemp, Warning, TEXT("Can't find function in %s : %s"), *FunctionName.ToString(), *GetName());
				continue;
			}
		
			EnhancedInputComponent->BindAction(InputActionPtr, TriggerEvent, this, FunctionName);
		}
	}
	

}

void AStaPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	EdgeScroll();
	UpdateHoveredActor();
}

void AStaPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	
	if(UEnhancedInputLocalPlayerSubsystem* EnhancedInputSubsystem =
				ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		EnhancedInputSubsystem->AddMappingContext(MappingContext, static_cast<int32>(EInputPriority::Controller));
	}
}

void AStaPlayerController::UpdateHoveredActor()
{
	FHitResult HitResult;
	GetHitResultUnderCursor(ECC_Visibility, false, HitResult);
	
	if (IInteractable* HitInteractable = Cast<IInteractable>(HitResult.GetActor()))
	{
		if (HoveredActor.IsValid())
		{
			if (HoveredActor != HitResult.GetActor())
			{
				IInteractable* HoveredInteractable = Cast<IInteractable>(HoveredActor);
				HoveredInteractable->OnHoverEnd();
				
				HitInteractable->OnHoverBegin();
				HoveredActor = HitResult.GetActor();
			}
		}
		else
		{
			HitInteractable->OnHoverBegin();
			HoveredActor = HitResult.GetActor();
		}
	}
	else
	{
		if (HoveredActor.IsValid())
		{
			IInteractable* HoveredInteractable = Cast<IInteractable>(HoveredActor);
			HoveredInteractable->OnHoverEnd();
			HoveredActor.Reset();
		}
	}

}

void AStaPlayerController::Interact(const FInputActionValue& Value)
{
	if (!HoveredActor.IsValid()) return;

	if (IInteractable* InteractableActor = Cast<IInteractable>(HoveredActor))
	{
		InteractableActor->OnInteract();
	}
}

void AStaPlayerController::Cancel(const FInputActionValue& Value)
{
	StaDebug::Print("Cancel Func");
}

void AStaPlayerController::Scroll(const FInputActionValue& Value)
{
	const float ScrollValue = Value.Get<float>();

	if (ACommandPawn* OwningPawn = Cast<ACommandPawn>(GetPawn()))
	{
		if (ScrollValue > 0.0f)
		{
			OwningPawn->SetScrollHeight(false);
		}
		else if (ScrollValue < 0.0f)
		{
			OwningPawn->SetScrollHeight(true);
		}
	}
	
}

void AStaPlayerController::EdgeScroll()
{
	ACommandPawn* CommandPawn = Cast<ACommandPawn>(GetPawn());
	if (!CommandPawn) return;
	
	float MouseX, MouseY;
	bool bCanEdgeScroll = GetMousePosition(MouseX, MouseY);

	if (!bCanEdgeScroll) return;

	int32 ViewportX, ViewportY;
	GetViewportSize(ViewportX, ViewportY);

	FVector Direction = FVector::ZeroVector;
	float EdgeSensitive = 120.0f;

	if (MouseX < EdgeSensitive)
	{
		Direction.Y = -1.0f;
	}
	else if (MouseX > ViewportX - EdgeSensitive)
	{
		Direction.Y = 1.0f ;
	}

	if (MouseY < EdgeSensitive)
	{
		Direction.X = 1.0f;
	}
	else if (MouseY > ViewportY - EdgeSensitive)
	{
		Direction.X = -1.0f;
	}

	if (Direction.IsNearlyZero()) return;

	if (!PlayerCameraManager) return;
	FRotator CameraRotation = PlayerCameraManager->GetCameraRotation();

	CommandPawn->MoveTo(Direction);
}
