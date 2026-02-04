// Sta Project - Multiplay RTS with GAS

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DiscardCard.generated.h"

class UCapsuleComponent;
/**
 * 
 */
UCLASS()
class STA_API ADiscardCard : public AActor
{
	GENERATED_BODY()

public:
	ADiscardCard();

protected:
	UPROPERTY(VisibleAnywhere, Category = "Component")
	UStaticMeshComponent* DiscardMesh;

};
