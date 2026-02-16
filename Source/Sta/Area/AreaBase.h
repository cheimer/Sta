// Sta Project - Multiplay RTS with GAS

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GenericTeamAgentInterface.h"
#include "GameFramework/Actor.h"
#include "Interface/Interactable.h"
#include "AreaBase.generated.h"

struct FOnAttributeChangeData;
class UNiagaraComponent;
class UNiagaraSystem;
class ALineBase;
class UAreaAttributeSet;
class UCapsuleComponent;
class AAreaBase;

DECLARE_MULTICAST_DELEGATE_ThreeParams(FOnAreaValueChanged, AAreaBase* AreaActor, const float UnitValue, const float DefenseValue);

UCLASS()
class STA_API AAreaBase : public AActor, public IInteractable, public IAbilitySystemInterface, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

public:
	AAreaBase();

	void AddLine(ALineBase* Line);

	TArray<AAreaBase*> GetConnectedArea();

	void SetHighlight(bool bIsHighlight);

	FOnAreaValueChanged OnAreaValueChanged;

	/**
	* IInteractable
	*/
	virtual void OnHoverBegin() override;
	virtual void OnHoverEnd() override;
	virtual void OnInteractBegin(const FHitResult& HitResult) override;
	virtual void OnInteracting(const FHitResult& HitResult) override;
	virtual void OnInteractEnd(const FHitResult& HitResult) override;
	virtual const TArray<FInteractOption>& GetInteractOptions() override;
	virtual FText GetInfoText() override;

	/**
	 * AbilitySystemInterface
	 */
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UAreaAttributeSet* GetAttributeSet() const;

	/**
	 * GenericTeamAgentInterface
	 */
	virtual void SetGenericTeamId(const FGenericTeamId& TeamID) override;
	virtual FGenericTeamId GetGenericTeamId() const override;

protected:
	virtual void BeginPlay() override;

	void OnUnitNumChanged(const FOnAttributeChangeData& Data);
	void OnDefenseChanged(const FOnAttributeChangeData& Data);

	UPROPERTY()
	TObjectPtr<APlayerState> OwningActor = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "Component")
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
	
	UPROPERTY()
	TObjectPtr<UAreaAttributeSet> AttributeSet;

	UPROPERTY(VisibleAnywhere, Category = "Component")
	UStaticMeshComponent* AreaMesh;

	UPROPERTY(EditAnywhere, Category = "VFX")
	UNiagaraSystem* HighlightVFX;

	UPROPERTY(EditAnywhere, Category = "VFX")
	FVector HighlightSpawnLocation = FVector(0.0f, 0.0f, 30.0f);

private:
	void SetInteractOptions();
	
	TArray<FInteractOption> Options;

	UPROPERTY()
	TArray<TObjectPtr<ALineBase>> ConnectLines;

	UPROPERTY(Transient)
	UNiagaraComponent* SpawnedHighlight;

	FGenericTeamId AreaTeamId;
	
};
