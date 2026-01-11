// Sta Project - Multiplay RTS with GAS


#include "CardBase.h"

#include "Component/CardComponent.h"
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

	//CostWidgetComp = CreateDefaultSubobject<UWidgetComponent>("CostWidgetComponent");
	//CostWidgetComp->SetupAttachment(CardMesh);
	
}

void ACardBase::BeginPlay()
{
	Super::BeginPlay();

	OriginalScale = GetActorScale();
	
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
	StaDebug::Print(FString::Printf(TEXT("%s is Interact Begin"), *GetName()));
	bIsInteracting = true;
}

void ACardBase::OnInteracting(const FHitResult& HitResult)
{
	FVector NewLocation = HitResult.Location;
	NewLocation.Z = GetActorLocation().Z;
	
	SetActorLocation(NewLocation);
}

void ACardBase::OnInteractEnd(const FHitResult& HitResult)
{
	StaDebug::Print(FString::Printf(TEXT("%s is Interact End"), *GetName()));

	if (!GetOwner()) return;
	UCardComponent* CardComp = GetOwner()->GetComponentByClass<UCardComponent>();
	if (!CardComp) return;

	CardComp->UseCard(this);
	bIsInteracting = false;
	
}
