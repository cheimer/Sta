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
	StaDebug::Print("InteractStart");
}

void AAreaBase::OnHoverEnd()
{
	StaDebug::Print("InteractEnd");
}

void AAreaBase::OnInteract()
{
	StaDebug::Print("InteractOn");
}
