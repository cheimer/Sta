// Sta Project - Multiplay RTS with GAS


#include "DiscardCard.h"

#include "Sta.h"
#include "Components/CapsuleComponent.h"

ADiscardCard::ADiscardCard()
{
	PrimaryActorTick.bCanEverTick = false;

	bReplicates = false;

	DiscardMesh = CreateDefaultSubobject<UStaticMeshComponent>("DiscardMesh");
	SetRootComponent(DiscardMesh);
	
	DiscardMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	DiscardMesh->SetCollisionObjectType(ECC_Area);
	DiscardMesh->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	
	DiscardMesh->SetCastShadow(false);
	DiscardMesh->SetEnableGravity(false);
	
}
