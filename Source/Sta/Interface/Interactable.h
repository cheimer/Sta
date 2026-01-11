// Sta Project - Multiplay RTS with GAS

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Interactable.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UInteractable : public UInterface
{
	GENERATED_BODY()
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
	virtual bool CanInteract() const { return true; }
	
};
