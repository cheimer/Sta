// Sta Project - Multiplay RTS with GAS


#include "CardComponent.h"

#include "Card/CardBase.h"


UCardComponent::UCardComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}

void UCardComponent::BeginPlay()
{
	Super::BeginPlay();
	
	
}

void UCardComponent::CreateCard()
{
	if (Cards.IsEmpty()) return;

	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if (!OwnerPawn || !OwnerPawn->IsLocallyControlled()) return;
	
	if (Cards[0].CardClass)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = OwnerPawn;
		GetWorld()->SpawnActor<ACardBase>(Cards[0].CardClass, OwnerPawn->GetActorLocation(), FRotator::ZeroRotator, SpawnParams);
	}
}
