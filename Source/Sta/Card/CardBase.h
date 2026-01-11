// Sta Project - Multiplay RTS with GAS

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/Interactable.h"
#include "CardBase.generated.h"

class UCardData;
class UWidgetComponent;

UCLASS()
class STA_API ACardBase : public AActor, public IInteractable
{
	GENERATED_BODY()

public:
	ACardBase();

	/**
	 * Interactable Interface
	 */
	virtual void OnHoverBegin() override;
	virtual void OnHoverEnd() override;
	virtual void OnInteractBegin(const FHitResult& HitResult) override;
	virtual void OnInteracting(const FHitResult& HitResult) override;
	virtual void OnInteractEnd(const FHitResult& HitResult) override;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = "Component")
	UStaticMeshComponent* CardMesh;

	//UPROPERTY(VisibleAnywhere, Category = "Component")
	//UWidgetComponent* CostWidgetComp;
	
	UPROPERTY(EditAnywhere, Category = "Sta|Value")
	float HoveringScale = 1.5f;

	UPROPERTY(EditDefaultsOnly, Category = "Sta|Value")
	TObjectPtr<UCardData> CardData;
	
	FVector OriginalScale;

	bool bIsInteracting = false;

public:
	bool GetIsInteracting() const { return bIsInteracting; }
	TObjectPtr<UCardData> GetCardData() const { return CardData; }

};
