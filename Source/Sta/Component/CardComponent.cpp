// Sta Project - Multiplay RTS with GAS


#include "CardComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "HairStrandsInterface.h"
#include "Area/DiscardCard.h"
#include "Card/CardBase.h"
#include "DataAsset/CardData.h"
#include "GameplayTag/StaTags.h"
#include "Helper/StaHelper.h"
#include "Character/CommandPawn.h"


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

void UCardComponent::CreateDiscardCard()
{
	if (!DiscardClass || !GetWorld()) return;
	
	ACommandPawn* OwnerCommand = Cast<ACommandPawn>(GetOwner());
	if (!OwnerCommand) return;

	APlayerController* PlayerController = Cast<APlayerController>(OwnerCommand->GetController());
	if (!PlayerController || !PlayerController->IsLocalController()) return;

	int SizeX, SizeY;
	PlayerController->GetViewportSize(SizeX, SizeY);

	FVector SpawnLocation = GetComponentLocation() + FVector(DiscardDistance, 0.0f);
	
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = GetOwner();

	DiscardCardActor = GetWorld()->SpawnActor<ADiscardCard>(DiscardClass, SpawnLocation, FRotator::ZeroRotator, SpawnParams);
	DiscardCardActor->AttachToComponent(this, FAttachmentTransformRules::KeepWorldTransform);
	
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

	for (int i = 0; i < HandCards.Num(); i++)
	{
		if (!HandCards[i] || HandCards[i]->GetIsInteracting()) continue;
        
		FVector Pos = Center + FVector::RightVector * (StartOffset + i * CardSpace);
		HandCards[i]->SetActorLocation(Pos);
	}

	if (!DiscardCardActor)
	{
		CreateDiscardCard();
	}

	
}

void UCardComponent::AddCardToHand(const UCardData* DrawCardData)
{
	if (!GetWorld() || !DrawCardData) return;

	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if (!OwnerPawn || !OwnerPawn->IsLocallyControlled()) return;
	
	if (!DrawCardData->CardClass) return;
	
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = OwnerPawn;
	
	ACardBase* SpawnedCard = GetWorld()->SpawnActor<ACardBase>(DrawCardData->CardClass, OwnerPawn->GetActorLocation(), FRotator::ZeroRotator, SpawnParams);
	SpawnedCard->SetCardData(DrawCardData);
	
	HandCards.Add(SpawnedCard);
	
}

void UCardComponent::RemoveCardFromHand(const UCardData* RemoveCardData)
{
	if (!RecentCard) return;
	if (RecentCard->GetCardData() != RemoveCardData) return;

	HandCards.RemoveSingle(RecentCard);

	RecentCard->Destroy();
}

void UCardComponent::UseCard(ACardBase* Card, AActor* Target)
{
	if (!GetOwner() || !Card || !Card->GetCardData()) return;

	UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetOwner());
	if (!ASC) return;

	FGameplayEventData EventData;
	EventData.Instigator = GetOwner();
	EventData.OptionalObject = Card->GetCardData();
	EventData.Target = Target;

	RecentCard = Card;
	
	ASC->HandleGameplayEvent(StaTags::Event::Card::Use, &EventData);
	
}

void UCardComponent::DiscardCard(ACardBase* Card)
{
	if (!GetOwner() || !Card || !Card->GetCardData()) return;

	UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetOwner());
	if (!ASC) return;
	
	FGameplayEventData EventData;
	EventData.Instigator = GetOwner();
	EventData.OptionalObject = Card->GetCardData();

	RecentCard = Card;

	ASC->HandleGameplayEvent(StaTags::Event::Card::Discard, &EventData);
	
}
