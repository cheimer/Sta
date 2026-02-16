// Sta Project - Multiplay RTS with GAS


#include "StaPlayerController.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "AbilitySystem/StaAbilitySystemComponent.h"
#include "AbilitySystem/AttributeSet/AreaAttributeSet.h"
#include "Area/AreaBase.h"
#include "Component/CardComponent.h"
#include "Framework/GameMode/StaGameModeBase.h"
#include "GameFramework/PlayerState.h"
#include "Helper/StaHelper.h"
#include "Interface/Interactable.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "Player/CommandPawn.h"
#include "UI/StaHUD.h"

AStaPlayerController::AStaPlayerController()
{
	bShowMouseCursor = true;
	PlayerTeamID = FGenericTeamId::NoTeam;
}

void AStaPlayerController::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, PlayerTeamID);
}

void AStaPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	if(UEnhancedInputLocalPlayerSubsystem* EnhancedInputSubsystem =
				ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		EnhancedInputSubsystem->AddMappingContext(MappingContext, static_cast<int32>(EInputPriority::Controller));
	}
	
	FInputModeGameAndUI CurrentInputMode;
	CurrentInputMode.SetLockMouseToViewportBehavior(EMouseLockMode::LockAlways);
	CurrentInputMode.SetHideCursorDuringCapture(false);
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

void AStaPlayerController::TriggerGameplayEvent(FGameplayTag GameplayTag, const FGameplayEventData* EventData)
{
	const IAbilitySystemInterface* AbilityInterface = Cast<IAbilitySystemInterface>(GetPawn());
	if (!AbilityInterface) return;

	UAbilitySystemComponent* PawnASC = AbilityInterface->GetAbilitySystemComponent();
	if (!PawnASC) return;

	if (HasAuthority())
	{
		PawnASC->HandleGameplayEvent(GameplayTag, EventData);
	}
	else
	{
		ServerTriggerGameplayEvent(GameplayTag, *EventData);
	}
	
}

void AStaPlayerController::ServerTriggerGameplayEvent_Implementation(FGameplayTag GameplayTag, const FGameplayEventData& EventData)
{
	TriggerGameplayEvent(GameplayTag, &EventData);
}

void AStaPlayerController::ClientDrawCard_Implementation(const UCardData* DrawCardData)
{
	if (!DrawCardData || !GetPawn()) return;
    
	UCardComponent* CardComp = GetPawn()->FindComponentByClass<UCardComponent>();
	if (!CardComp) return;
    
	CardComp->AddCardToHand(DrawCardData);
    
}

void AStaPlayerController::ClientDiscardCard_Implementation(const UCardData* RemoveCardData)
{
	if (!RemoveCardData || !GetPawn()) return;
    
	UCardComponent* CardComp = GetPawn()->FindComponentByClass<UCardComponent>();
	if (!CardComp) return;

	CardComp->RemoveCardFromHand(RemoveCardData);
}

void AStaPlayerController::ClientAreaValueChanged_Implementation(AAreaBase* AreaActor, const float UnitValue, const float DefenseValue)
{
	if (!AreaActor || !AreaActor->GetAttributeSet()) return;

	AreaActor->GetAttributeSet()->SetUnitNum(UnitValue);
	AreaActor->GetAttributeSet()->SetDefense(DefenseValue);
}

void AStaPlayerController::UpdateHoveredActor()
{
	if (bIsInteracting) return;
	
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

void AStaPlayerController::OnRep_Pawn()
{
	Super::OnRep_Pawn();

	TryBindingHUD();
	TryInitDeckList();
	
}

void AStaPlayerController::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	TryBindingHUD();
	TryInitDeckList();
}

void AStaPlayerController::TryBindingHUD()
{
	if (!IsLocalController()) return;

	if (bHUDBounding) return;

	if (!GetPawn() || !PlayerState) return;
	
	IAbilitySystemInterface* AbilitySystemInterface = Cast<IAbilitySystemInterface>(PlayerState);
	if (!AbilitySystemInterface) return;
	
	UAbilitySystemComponent* PawnASC = AbilitySystemInterface->GetAbilitySystemComponent();
	if (!PawnASC) return;
	
	AStaHUD* StaHUD = Cast<AStaHUD>(GetHUD());
	if (!StaHUD) return;
	
	StaHUD->SetASCBinding(PawnASC);
	bHUDBounding = true;
	
}

void AStaPlayerController::TryInitDeckList()
{
	if (!IsLocalController()) return;

	if (bInitDeckList) return;

	if (!GetPawn() || !PlayerState) return;

	UCardComponent* CardComp = GetPawn()->FindComponentByClass<UCardComponent>();
	if (!CardComp) return;

	TArray<FCardInfo> DeckList = CardComp->GetDeckList();
	if (DeckList.Num() == 0) return;

	ServerInitDeck(DeckList);
	bInitDeckList = true;
}

void AStaPlayerController::ServerInitDeck_Implementation(const TArray<FCardInfo>& DeckList)
{
	if (!GetWorld() || DeckList.Num() == 0) return;

	AStaGameModeBase* GameMode = Cast<AStaGameModeBase>(GetWorld()->GetAuthGameMode());
	if (!GameMode) return;

	GameMode->InitDeckState(DeckList, this);
}

void AStaPlayerController::SetControllerState(FGameplayTag NewStateTag, const TArray<FInteractOption>& NewOptions)
{
	OnControllerStateChanged.Broadcast(StateTag, NewStateTag, NewOptions);
	StateTag = NewStateTag;
}

void AStaPlayerController::SetControllerIdle()
{
	SetControllerState(StaTags::State::Controller::Idle);
}

void AStaPlayerController::SetControllerTargeting()
{
	SetControllerState(StaTags::State::Controller::Targeting);

	if (!RecentInteractActor.IsValid()) return;
	
	SetConnectedAreasHighlight(RecentInteractActor.Get(), true);
}

void AStaPlayerController::SetConnectedAreasHighlight(AActor* RootArea, const bool bIsHighlight)
{
	if (AAreaBase* InteractedArea = Cast<AAreaBase>(RootArea))
	{
		TArray<AAreaBase*> ConnectedAreas = InteractedArea->GetConnectedArea();
		for (AAreaBase* ConnectedArea : ConnectedAreas)
		{
			ConnectedArea->SetHighlight(bIsHighlight);
		}
	}
}

/**
 * GenericTeamAgentInterface
 */
void AStaPlayerController::SetGenericTeamId(const FGenericTeamId& TeamID)
{
	PlayerTeamID = TeamID;
}

FGenericTeamId AStaPlayerController::GetGenericTeamId() const
{
	return PlayerTeamID;
}

/**
 * Input Actions
 */

void AStaPlayerController::InteractBegin(const FInputActionValue& Value)
{
	IInteractable* InteractableActor = Cast<IInteractable>(HoveredActor);
	if (!InteractableActor) return;
	
	FHitResult HitResult;
	GetHitResultUnderCursor(ECC_Visibility, false, HitResult);

	bIsInteracting = true;
	
	if (StateTag == StaTags::State::Controller::Targeting)
	{
		//
	}
	else
	{
		const TArray<FInteractOption>& Options = InteractableActor->GetInteractOptions();
		if (Options.IsEmpty()) return;
	
		InteractableActor->OnInteractBegin(HitResult);

		if (Options[0].InteractTag.MatchesTag(StaTags::Interaction::Card_Root))
		{
			SetControllerState(StaTags::State::Controller::Drag, Options);
		}

		RecentInteractActor = HoveredActor;
		
	}
	
}

void AStaPlayerController::Interacting(const FInputActionValue& Value)
{
	if (IInteractable* InteractableActor = Cast<IInteractable>(HoveredActor))
	{
		FHitResult HitResult;
		GetHitResultUnderCursor(ECC_Visibility, false, HitResult);
	
		InteractableActor->OnInteracting(HitResult);
	}
}

void AStaPlayerController::InteractEnd(const FInputActionValue& Value)
{
	IInteractable* InteractableActor = Cast<IInteractable>(HoveredActor);
	if (!InteractableActor) return;
	
	FHitResult HitResult;
	GetHitResultUnderCursor(ECC_Visibility, false, HitResult);

	bIsInteracting = false;
	
	if (StateTag == StaTags::State::Controller::Targeting)
	{
		AAreaBase* HoveredArea = Cast<AAreaBase>(HoveredActor);
		if (!RecentInteractActor.IsValid() || !HoveredArea) return;

		bool bIsCorrectArea = false;
		TArray<AAreaBase*> HoveredConnectedAreas = HoveredArea->GetConnectedArea();
		for (AAreaBase* HoveredConnectedArea : HoveredConnectedAreas)
		{
			if (HoveredConnectedArea == RecentInteractActor.Get())
			{
				bIsCorrectArea = true;
				break;
			}
		}

		if (!bIsCorrectArea) return;

		UAbilitySystemComponent* InteractActorASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(RecentInteractActor.Get());
		if (!InteractActorASC) return;

		FGameplayEventData MoveEventData;
		
		FGameplayAbilityTargetData_ActorArray* AreaArray = new FGameplayAbilityTargetData_ActorArray();
		AreaArray->TargetActorArray.Add(RecentInteractActor.Get());
		AreaArray->TargetActorArray.Add(HoveredActor.Get());
		
		MoveEventData.TargetData.Add(AreaArray);
		//TODO: Temp move all
		MoveEventData.EventMagnitude = InteractActorASC->GetNumericAttribute(UAreaAttributeSet::GetUnitNumAttribute());
		
		TriggerGameplayEvent(StaTags::Event::Area::Move, &MoveEventData);
		
		SetConnectedAreasHighlight(RecentInteractActor.Get(), false);

		SetControllerIdle();
	}
	else
	{
		const TArray<FInteractOption>& Options = InteractableActor->GetInteractOptions();
		if (Options.IsEmpty()) return;
	
		InteractableActor->OnInteractEnd(HitResult);

		if (Options[0].InteractTag.MatchesTag(StaTags::Interaction::Card_Root) && StateTag.MatchesTagExact(StaTags::State::Controller::Drag))
		{
			SetControllerState(StaTags::State::Controller::Idle, Options);
		}
		else if (Options[0].InteractTag.MatchesTag(StaTags::Interaction::Area_Root) && StateTag.MatchesTagExact(StaTags::State::Controller::Idle))
		{
			SetControllerState(StaTags::State::Controller::Menu, Options);
		}
		
	}
}

void AStaPlayerController::Cancel(const FInputActionValue& Value)
{
	if (bIsInteracting) return;

	OnControllerCanceled.Broadcast();

	if (StateTag == StaTags::State::Controller::Targeting)
	{
		SetControllerState(StaTags::State::Controller::Idle);

		if (RecentInteractActor.IsValid())
		{
			SetConnectedAreasHighlight(RecentInteractActor.Get(), false);
		}
	}
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

void AStaPlayerController::Move(const FInputActionValue& Value)
{
	FVector2D MoveValue = Value.Get<FVector2D>();
	
	if (ACommandPawn* OwningPawn = Cast<ACommandPawn>(GetPawn()))
	{
		OwningPawn->MoveTo(FVector(MoveValue, 0.0f));
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

	CommandPawn->MoveTo(Direction);
}
