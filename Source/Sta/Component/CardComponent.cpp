// Sta Project - Multiplay RTS with GAS


#include "CardComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "HairStrandsInterface.h"
#include "Card/CardBase.h"
#include "DataAsset/CardData.h"
#include "GameplayTag/StaTags.h"
#include "Helper/StaHelper.h"
#include "Player/CommandPawn.h"


UCardComponent::UCardComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}

void UCardComponent::BeginPlay()
{
	Super::BeginPlay();
	
	ACommandPawn* OwnerCommand = Cast<ACommandPawn>(GetOwner());
	if (!OwnerCommand) return;

	DefaultArmLength = OwnerCommand->GetTargetArmLength();
}

void UCardComponent::UpdateCardOffset()
{
	if (HandCards.IsEmpty()) return;
	ACommandPawn* OwnerCommand = Cast<ACommandPawn>(GetOwner());
	if (!OwnerCommand) return;

	APlayerController* PlayerController = Cast<APlayerController>(OwnerCommand->GetController());
	if (!PlayerController || !PlayerController->IsLocalController()) return;

	int SizeX, SizeY;
	PlayerController->GetViewportSize(SizeX, SizeY);

	FVector WorldLocation, WorldDirection;
	bool bIsSuccess = PlayerController->DeprojectScreenPositionToWorld(SizeX * CardOffset.X, SizeY * CardOffset.Y, WorldLocation, WorldDirection);
	if (!bIsSuccess) return;

	float DifTargetArmLen = DefaultArmLength - OwnerCommand->GetTargetArmLength();

	float Len = ((CardHeight - DifTargetArmLen) - WorldLocation.Z) / WorldDirection.Z;
	SetWorldLocation(WorldLocation + Len * WorldDirection);

	FVector Center = GetComponentLocation();
	float StartOffset = -(HandCards.Num() - 1) * CardSpace * 0.5f;

	for (int32 i = 0; i < HandCards.Num(); i++)
	{
		if (!HandCards[i] || HandCards[i]->GetIsInteracting()) continue;
        
		FVector Pos = Center + FVector::RightVector * (StartOffset + i * CardSpace);
		HandCards[i]->SetActorLocation(Pos);
	}
	
}

void UCardComponent::CreateCard()
{
	if (!GetWorld() || CardInfos.IsEmpty()) return;

	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if (!OwnerPawn || !OwnerPawn->IsLocallyControlled()) return;

	for (auto CardInfo : CardInfos)
	{
		if (CardInfo.CardClass)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = OwnerPawn;
			for (int i = 0; i < CardInfo.CardNum; i++)
			{
				HandCards.Add(GetWorld()->SpawnActor<ACardBase>(CardInfo.CardClass, OwnerPawn->GetActorLocation(), FRotator::ZeroRotator, SpawnParams));
			}
		}
	}
	
}

void UCardComponent::UseCard(ACardBase* Card, AActor* Target)
{
	if (!GetOwner() || !Card || !Card->GetCardData()) return;

	UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetOwner());
	if (!ASC) return;

	FGameplayEventData EventData;
	EventData.OptionalObject = Card;
	EventData.Target = Target;
	ASC->HandleGameplayEvent(StaTags::Event::Card::Use, &EventData);
	
}
