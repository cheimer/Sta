// Sta Project - Multiplay RTS with GAS


#include "CardBase.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Components/WidgetComponent.h"
#include "DataAsset/CardData.h"
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

	//CostWidgetComp = CreateDefaultSubobject<UWidgetComponent>("CostWidgetComponent");
	//CostWidgetComp->SetupAttachment(CardMesh);
	
}

void ACardBase::BeginPlay()
{
	Super::BeginPlay();

	OriginalLocation = GetActorLocation();
	OriginalScale = GetActorScale();
	
}

void ACardBase::OnHoverBegin()
{
	SetActorLocation(OriginalLocation + HoveringDistance);
	SetActorScale3D(OriginalScale * HoveringScale);
}

void ACardBase::OnHoverEnd()
{
	if (!bIsInteracting)
	{
		SetActorLocation(OriginalLocation);
		SetActorScale3D(OriginalScale);
	}
	
}

void ACardBase::OnInteractBegin(const FHitResult& HitResult)
{
	StaDebug::Print(FString::Printf(TEXT("%s is Interact Begin"), *GetName()));
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

	UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetOwner());
	if (!ASC || !CardData) return;

	bIsInteracting = true;
	
	FGameplayEventData EventData;
	EventData.OptionalObject = CardData;

	ASC->HandleGameplayEvent(StaTags::Event::Card::Use, &EventData);
	
	
}
