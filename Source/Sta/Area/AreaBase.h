// Sta Project - Multiplay RTS with GAS

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/Interactable.h"
#include "AreaBase.generated.h"

UCLASS()
class STA_API AAreaBase : public AActor, public IInteractable
{
	GENERATED_BODY()

public:
	AAreaBase();

/**
 * IInteractable
 */
	virtual void OnHoverBegin() override;
	virtual void OnHoverEnd() override;
	virtual void OnInteractBegin(const FHitResult& HitResult) override;
	virtual void OnInteracting(const FHitResult& HitResult) override;
	virtual void OnInteractEnd(const FHitResult& HitResult) override;

protected:
	virtual void BeginPlay() override;
	

};
