// Sta Project - Multiplay RTS with GAS


#include "AreaBase.h"

#include "Helper/StaHelper.h"


AAreaBase::AAreaBase()
{
	PrimaryActorTick.bCanEverTick = false;
	
}

void AAreaBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void AAreaBase::OnHoverBegin()
{
	StaDebug::Print("Area Hover Start");
}

void AAreaBase::OnHoverEnd()
{
	StaDebug::Print("Area Hover End");
}

void AAreaBase::OnInteractBegin(const FHitResult& HitResult)
{
	StaDebug::Print("Area Interact Begin");
}

void AAreaBase::OnInteracting(const FHitResult& HitResult)
{
	StaDebug::Print("Area Interacting");
}

void AAreaBase::OnInteractEnd(const FHitResult& HitResult)
{
	StaDebug::Print("Area Interact End");
}
