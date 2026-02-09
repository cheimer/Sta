// Sta Project - Multiplay RTS with GAS

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/Interface.h"
#include "Interactable.generated.h"

// This class does not need to be modified.
UINTERFACE(BlueprintType)
class UInteractable : public UInterface
{
	GENERATED_BODY()
};

USTRUCT()
struct FInteractOption
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FText DisplayName;

	UPROPERTY(EditAnywhere, Meta = (Categories = "Interaction"))
	FGameplayTag InteractTag;

	TWeakObjectPtr<AActor> TargetActor;

};

/**
 * 
 */
class STA_API IInteractable
{
	GENERATED_BODY()

public:
	virtual void OnHoverBegin() = 0;
	virtual void OnHoverEnd() = 0;
	virtual void OnInteractBegin(const FHitResult& HitResult) = 0;
	virtual void OnInteracting(const FHitResult& HitResult) = 0;
	virtual void OnInteractEnd(const FHitResult& HitResult) = 0;
	virtual const TArray<FInteractOption>& GetInteractOptions() = 0;
	
	virtual bool CanInteract() const { return true; }
	virtual FText GetInfoText() { return FText(); }
	
};
