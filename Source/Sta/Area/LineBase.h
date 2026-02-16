// Sta Project - Multiplay RTS with GAS

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LineBase.generated.h"

class AAreaBase;

UCLASS()
class STA_API ALineBase : public AActor
{
	GENERATED_BODY()

public:
	ALineBase();

	AAreaBase* GetConnectArea(AAreaBase* InArea);

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = "Component")
	UStaticMeshComponent* LineMesh;

	UPROPERTY(EditInstanceOnly, Category = "Value")
	TArray<TObjectPtr<AAreaBase>> NodeAreas;

	UPROPERTY(EditInstanceOnly, Category = "Value")
	float MoveTime = -1.0f;

};
