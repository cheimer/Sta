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
	virtual void OnInteract() override;

protected:
	virtual void BeginPlay() override;
	

};
