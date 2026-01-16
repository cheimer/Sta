// Sta Project - Multiplay RTS with GAS

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Actor.h"
#include "Interface/Interactable.h"
#include "AreaBase.generated.h"

class UAreaAttributeSet;
class UCapsuleComponent;

UCLASS()
class STA_API AAreaBase : public AActor, public IInteractable, public IAbilitySystemInterface
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

	/**
	 * AbilitySystemInterface
	 */
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	
	UAreaAttributeSet* GetAttributeSet() const;

protected:
	virtual void BeginPlay() override;

	UPROPERTY()
	TObjectPtr<APlayerState> OwningActor = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "Component")
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
	
	UPROPERTY()
	TObjectPtr<UAreaAttributeSet> AttributeSet;

	UPROPERTY(VisibleAnywhere, Category = "Component")
	UStaticMeshComponent* AreaMesh;
	
};
