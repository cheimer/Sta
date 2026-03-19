// Sta Project - Multiplay RTS with GAS


#include "StaPlayerController.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "AbilitySystem/AttributeSet/AreaAttributeSet.h"
#include "Area/AreaBase.h"
#include "Component/CardComponent.h"
#include "Framework/GameMode/StaGameModeBase.h"
#include "GameFramework/PlayerState.h"
#include "Interface/Interactable.h"
#include "Character/CommandPawn.h"
#include "Kismet/GameplayStatics.h"
#include "UI/StaHUD.h"

AStaPlayerController::AStaPlayerController()
{
	bShowMouseCursor = true;
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
	AreaActor->SetLastScanTime();

	AreaActor->SetTextRenderComponent();
}

void AStaPlayerController::ClientAreaBluffChanged_Implementation(AAreaBase* AreaActor, const float BluffUnitAdd, const float BluffDefenseAdd)
{
	if (!AreaActor || !AreaActor->GetAttributeSet()) return;

	AreaActor->GetAttributeSet()->SetBluffUnitAdd(BluffUnitAdd);
	AreaActor->GetAttributeSet()->SetBluffDefenseAdd(BluffDefenseAdd);
	
	AreaActor->SetTextRenderComponent();
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
	SetHUDColor();

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

void AStaPlayerController::SetHUDColor()
{
	IGenericTeamAgentInterface* TeamAgent = Cast<IGenericTeamAgentInterface>(PlayerState);
	if (!TeamAgent) return;
	
	AStaHUD* StaHUD = Cast<AStaHUD>(GetHUD());
	if (!StaHUD) return;
	
	AAreaBase* RandArea = Cast<AAreaBase>(UGameplayStatics::GetActorOfClass(this, AAreaBase::StaticClass()));
	if (!RandArea) return;

	UTeamPaletteData* TeamPaletteData = RandArea->GetTeamPaletteData();
	if (!TeamPaletteData || TeamPaletteData->PaletteColor.Num() <= TeamAgent->GetGenericTeamId().GetId()) return;
	
	StaHUD->SetTeamColor(TeamPaletteData->PaletteColor[TeamAgent->GetGenericTeamId().GetId()]);
	
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

	if(StateTag == StaTags::State::Controller::Targeting)
	{
		if (RecentInteractActor.IsValid())
		{
			SetConnectedAreasHighlight(RecentInteractActor.Get(), false);
		}
	}
	else if (StateTag == StaTags::State::Controller::Menu)
	{
		if (NewStateTag == StaTags::State::Controller::Idle)
		{
			if (RecentInteractActor.IsValid())
			{
				SetConnectedAreasHighlight(RecentInteractActor.Get(), false);
			}
		}
	}
	else if (StateTag == StaTags::State::Controller::Employing)
	{
		SetMyAreasHighlight(false);
	}
	
	StateTag = NewStateTag;

	if (StateTag == StaTags::State::Controller::Targeting)
	{
		if (RecentInteractActor.IsValid())
		{
			SetConnectedAreasHighlight(RecentInteractActor.Get(), true);
		}
	}
	else if (StateTag == StaTags::State::Controller::Employing)
	{
		SetMyAreasHighlight(true);
	}
}

void AStaPlayerController::SetControllerIdle()
{
	SetControllerState(StaTags::State::Controller::Idle);
}

void AStaPlayerController::SetControllerTargeting()
{
	SetControllerState(StaTags::State::Controller::Targeting);

}

void AStaPlayerController::SetControllerEmploying()
{
	SetControllerState(StaTags::State::Controller::Employing);
}

void AStaPlayerController::ScanInteractingArea()
{
	if (!RecentInteractActor.IsValid()) return;

	FGameplayEventData ScanEventData;
		
	FGameplayAbilityTargetData_ActorArray* AreaArray = new FGameplayAbilityTargetData_ActorArray();
	AreaArray->TargetActorArray.Add(RecentInteractActor.Get());
		
	ScanEventData.TargetData.Add(AreaArray);
	
	TriggerGameplayEvent(StaTags::Event::Area::Scan, &ScanEventData);
		
}

void AStaPlayerController::MoveUnit(const float UnitNum)
{
	if (!RecentInteractActor.IsValid() || !RecentTargetActor.IsValid()) return;
	
	UAbilitySystemComponent* InteractActorASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(RecentInteractActor.Get());
	if (!InteractActorASC) return;

	FGameplayEventData MoveEventData;
	
	FGameplayAbilityTargetData_ActorArray* AreaArray = new FGameplayAbilityTargetData_ActorArray();
	AreaArray->TargetActorArray.Add(RecentInteractActor.Get());
	AreaArray->TargetActorArray.Add(RecentTargetActor.Get());
	
	MoveEventData.TargetData.Add(AreaArray);
	MoveEventData.EventMagnitude = UnitNum;

	IGenericTeamAgentInterface* RecentInteractTeam = Cast<IGenericTeamAgentInterface>(RecentInteractActor);
	IGenericTeamAgentInterface* RecentTargetTeam = Cast<IGenericTeamAgentInterface>(RecentTargetActor);
	if (RecentInteractTeam->GetGenericTeamId() == RecentTargetTeam->GetGenericTeamId())
	{
		TriggerGameplayEvent(StaTags::Event::Area::Move, &MoveEventData);
	}
	else
	{
		TriggerGameplayEvent(StaTags::Event::Area::Attack, &MoveEventData);
	}
	
	SetConnectedAreasHighlight(RecentInteractActor.Get(), false);

	SetControllerIdle();
}

void AStaPlayerController::EmployUnit(AAreaBase* TargetArea)
{
	if (!TargetArea) return;
	
	FGameplayEventData EventData;
	FGameplayAbilityTargetData_ActorArray* TargetData = new FGameplayAbilityTargetData_ActorArray();
	TargetData->TargetActorArray.Add(TargetArea);
	EventData.TargetData.Add(TargetData);
	EventData.EventMagnitude = UnitEmployNum;
	
	TriggerGameplayEvent(StaTags::Event::Area::Employ, &EventData);
}

float AStaPlayerController::GetInteractAreaUnitNum()
{
	if (!RecentInteractActor.IsValid()) return 0.0f;

	AAreaBase* InteractArea = Cast<AAreaBase>(RecentInteractActor);
	if (!InteractArea) return 0.0f;

	return InteractArea->GetAttributeSet()->GetUnitNum();
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

void AStaPlayerController::SetMyAreasHighlight(const bool bIsHighlight)
{
	IGenericTeamAgentInterface* StateTeamID = Cast<IGenericTeamAgentInterface>(PlayerState);
	if (!StateTeamID) return;
	
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(this, AAreaBase::StaticClass(), FoundActors);

	for (AActor* FoundActor : FoundActors)
	{
		if (AAreaBase* FoundArea = Cast<AAreaBase>(FoundActor))
		{
			if (FoundArea->GetGenericTeamId() == StateTeamID->GetGenericTeamId())
			{
				FoundArea->SetHighlight(bIsHighlight);
			}
		}
	}
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
		FGenericTeamId OwningTeamId = FGenericTeamId::NoTeam;
		if (IGenericTeamAgentInterface* StateTeamID = Cast<IGenericTeamAgentInterface>(PlayerState))
		{
			OwningTeamId = StateTeamID->GetGenericTeamId();
		}
		const TArray<FInteractOption>& Options = InteractableActor->GetInteractOptions(OwningTeamId);
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
		AAreaBase* RecentArea = Cast<AAreaBase>(RecentInteractActor);
		if (!RecentArea || !HoveredArea) return;

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

		RecentTargetActor = HoveredActor;
	}
	
	FGenericTeamId OwningTeamId = FGenericTeamId::NoTeam;
	if (IGenericTeamAgentInterface* StateTeamID = Cast<IGenericTeamAgentInterface>(PlayerState))
	{
		OwningTeamId = StateTeamID->GetGenericTeamId();
	}
	
	const TArray<FInteractOption>& Options = InteractableActor->GetInteractOptions(OwningTeamId);
	if (Options.IsEmpty()) return;

	InteractableActor->OnInteractEnd(HitResult);

	if (Options[0].InteractTag.MatchesTag(StaTags::Interaction::Card_Root))
	{
		if (StateTag.MatchesTagExact(StaTags::State::Controller::Drag))
		{
			SetControllerState(StaTags::State::Controller::Idle, Options);
		}
	}
	else if (Options[0].InteractTag.MatchesTag(StaTags::Interaction::Area_Root))
	{
		if (StateTag.MatchesTagExact(StaTags::State::Controller::Idle))
		{
			SetControllerState(StaTags::State::Controller::Menu, Options);
		}
		else if (StateTag.MatchesTagExact(StaTags::State::Controller::Targeting))
		{
			if (IInteractable* BeforeInteract = Cast<IInteractable>(RecentInteractActor))
			{
				const TArray<FInteractOption>& BeforeInteractOptions = BeforeInteract->GetInteractOptions(OwningTeamId);
				if (Options.IsEmpty()) return;
				
				SetControllerState(StaTags::State::Controller::Menu, BeforeInteractOptions);
				
			}
		}
		else if (StateTag.MatchesTagExact(StaTags::State::Controller::Employing))
		{
			AAreaBase* HoveredArea = Cast<AAreaBase>(HoveredActor);
			if (HoveredArea && OwningTeamId != FGenericTeamId::NoTeam && HoveredArea->GetGenericTeamId() == OwningTeamId)
			{
				EmployUnit(HoveredArea);
				SetControllerState(StaTags::State::Controller::Idle);
			}
		}
	}
	
	
}

void AStaPlayerController::Cancel(const FInputActionValue& Value)
{
	if (bIsInteracting) return;

	OnControllerCanceled.Broadcast();

	if (StateTag == StaTags::State::Controller::Targeting || StateTag == StaTags::State::Controller::Employing)
	{
		SetControllerState(StaTags::State::Controller::Idle);
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
