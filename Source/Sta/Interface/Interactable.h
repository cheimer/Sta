// Sta Project - Multiplay RTS with GAS

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GenericTeamAgentInterface.h"
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

	FLinearColor DisplayColor = FLinearColor::Black;
	
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
	virtual const TArray<FInteractOption>& GetInteractOptions(FGenericTeamId Interactor) { static const TArray<FInteractOption> Empty; return Empty; }
	
	virtual bool CanInteract() const { return true; }
	virtual FText GetInfoText(FGenericTeamId Interactor) { return FText(); }
	
};
