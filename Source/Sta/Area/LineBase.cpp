// Sta Project - Multiplay RTS with GAS


#include "LineBase.h"

#include "AreaBase.h"


ALineBase::ALineBase()
{
	PrimaryActorTick.bCanEverTick = false;
	
	LineMesh = CreateDefaultSubobject<UStaticMeshComponent>("LineMesh");
	SetRootComponent(LineMesh);
	
	LineMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
}

AAreaBase* ALineBase::GetConnectArea(AAreaBase* InArea)
{
	if (!InArea || NodeAreas.Num() < 2 || !NodeAreas[0] || !NodeAreas[1]) return nullptr;

	if (NodeAreas[0] == InArea) return NodeAreas[1];
	if (NodeAreas[1] == InArea) return NodeAreas[0];

	return nullptr;
}

void ALineBase::BeginPlay()
{
	Super::BeginPlay();

	check(NodeAreas.Num() == 2);

	for (auto NodeArea : NodeAreas)
	{
		NodeArea->AddLine(this);
	}
	
	
}
