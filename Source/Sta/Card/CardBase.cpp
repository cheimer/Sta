// Sta Project - Multiplay RTS with GAS


#include "CardBase.h"

#include "Sta.h"
#include "Area/AreaBase.h"
#include "Area/DiscardCard.h"
#include "Camera/CameraComponent.h"
#include "Component/CardComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "GameplayTag/StaTags.h"
#include "Helper/StaHelper.h"


ACardBase::ACardBase()
{
	PrimaryActorTick.bCanEverTick = false;

	bReplicates = false;

	CardMesh = CreateDefaultSubobject<UStaticMeshComponent>("CardMesh");
	SetRootComponent(CardMesh);
	CardMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CardMesh->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	CardMesh->SetCastShadow(false);

	CardCollision = CreateDefaultSubobject<UCapsuleComponent>("CardCollision");
	CardCollision->SetupAttachment(CardMesh);
	CardCollision->SetCapsuleHalfHeight(4000.0f);
	CardCollision->SetCapsuleRadius(80.0f);
	
	CardCollision->SetEnableGravity(false);
	CardCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CardCollision->SetCollisionResponseToAllChannels(ECR_Ignore);
	CardCollision->SetCollisionResponseToChannel(ECC_Area, ECR_Overlap);
	
}

void ACardBase::BeginPlay()
{
	Super::BeginPlay();

	OriginalScale = GetActorScale();

	if (GetOwner())
	{
		OwnerPawn = Cast<APawn>(GetOwner());
	}

	SetOptions();
}

void ACardBase::SetOptions()
{
	FInteractOption InteractOption;
	InteractOption.DisplayName = FText::GetEmpty();
	InteractOption.InteractTag = StaTags::Interaction::Card::None;

	Options.Add(InteractOption);
	
}

void ACardBase::OnHoverBegin()
{
	SetActorScale3D(OriginalScale * HoveringScale);
}

void ACardBase::OnHoverEnd()
{
	SetActorScale3D(OriginalScale);
	
}

void ACardBase::OnInteractBegin(const FHitResult& HitResult)
{
	bIsInteracting = true;
}

void ACardBase::OnInteracting(const FHitResult& HitResult)
{
	FVector NewLocation = HitResult.Location;
	NewLocation.Z = GetActorLocation().Z;
	
	SetActorLocation(NewLocation);

	if (!OwnerPawn.IsValid() || !CardCollision) return;
	APlayerController* PlayerController = Cast<APlayerController>(OwnerPawn->GetController());
	if (!PlayerController) return;

	FVector WorldLocation, WorldDirection;
	PlayerController->DeprojectMousePositionToWorld(WorldLocation, WorldDirection);

	FMatrix RotMatrix = FRotationMatrix::MakeFromZ(WorldDirection);
	CardCollision->SetWorldRotation(RotMatrix.Rotator());

}

void ACardBase::OnInteractEnd(const FHitResult& HitResult)
{
	if (!GetOwner() || !CardCollision) return;
	UCardComponent* CardComp = GetOwner()->GetComponentByClass<UCardComponent>();
	if (!CardComp) return;

	TArray<AActor*> OverlappedDiscard;
	CardCollision->GetOverlappingActors(OverlappedDiscard, ADiscardCard::StaticClass());
	if (OverlappedDiscard.Num() > 0)
	{
		CardComp->DiscardCard(this);
		return;
	}

	AActor* TargetArea = nullptr;
	TArray<AActor*> OverlappedAreas;
	CardCollision->GetOverlappingActors(OverlappedAreas, AAreaBase::StaticClass());

	if (OverlappedAreas.Num() == 0)
	{
		bIsInteracting = false;
		return;
	}
	else if (OverlappedAreas.Num() == 1)
	{
		TargetArea = OverlappedAreas[0];
	}
	else
	{
		float MinDistance = TNumericLimits<float>::Max();
		for (auto OverlapActor : OverlappedAreas)
		{
			FVector ClosestLocation;
			float Distance = CardCollision->GetClosestPointOnCollision(OverlapActor->GetActorLocation(), ClosestLocation);

			if (Distance < MinDistance)
			{
				MinDistance = Distance;
				TargetArea = OverlapActor;
			}
		}
	}
	
	CardComp->UseCard(this, TargetArea);
	bIsInteracting = false;
	
}

const TArray<FInteractOption>& ACardBase::GetInteractOptions()
{
	return Options;
}

TArray<AActor*> ACardBase::GetOverlappedActors()
{
	TArray<AActor*> Overlapped;
	CardCollision->GetOverlappingActors(Overlapped, AAreaBase::StaticClass());

	return Overlapped;
}
