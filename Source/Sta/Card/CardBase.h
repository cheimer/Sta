// Sta Project - Multiplay RTS with GAS

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/Interactable.h"
#include "CardBase.generated.h"

class UCapsuleComponent;
class UCardData;
class UWidgetComponent;

UCLASS()
class STA_API ACardBase : public AActor, public IInteractable
{
	GENERATED_BODY()

public:
	ACardBase();

	TArray<AActor*> GetOverlappedActors();

	/**
	 * Interactable Interface
	 */
	virtual void OnHoverBegin() override;
	virtual void OnHoverEnd() override;
	virtual void OnInteractBegin(const FHitResult& HitResult) override;
	virtual void OnInteracting(const FHitResult& HitResult) override;
	virtual void OnInteractEnd(const FHitResult& HitResult) override;
	virtual const TArray<FInteractOption>& GetInteractOptions() override;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = "Component")
	UStaticMeshComponent* CardMesh;

	UPROPERTY(VisibleAnywhere, Category = "Component")
	UCapsuleComponent* CardCollision;

	UPROPERTY(EditAnywhere, Category = "Sta|Value")
	float HoveringScale = 1.5f;

	UPROPERTY()
	TObjectPtr<const UCardData> CardData;
	
	FVector OriginalScale;

	bool bIsInteracting = false;

private:
	void SetOptions();
	
	TWeakObjectPtr<APawn> OwnerPawn;

	TArray<FInteractOption> Options;
	
public:
	bool GetIsInteracting() const { return bIsInteracting; }
	TObjectPtr<const UCardData> GetCardData() const { return CardData; }
	void SetCardData(const UCardData* InCardData) { CardData = InCardData; }

};
