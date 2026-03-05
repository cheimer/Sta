// Sta Project - Multiplay RTS with GAS

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GenericTeamAgentInterface.h"
#include "DataAsset/TeamPaletteData.h"
#include "GameFramework/Actor.h"
#include "Interface/Interactable.h"
#include "AreaBase.generated.h"

class UGameplayEffect;
class UTextRenderComponent;
struct FOnAttributeChangeData;
class UNiagaraComponent;
class UNiagaraSystem;
class ALineBase;
class UAreaAttributeSet;
class UCapsuleComponent;
class AAreaBase;

DECLARE_MULTICAST_DELEGATE_ThreeParams(FOnAreaValueChanged, AAreaBase* AreaActor, const float UnitValue, const float DefenseValue);
DECLARE_MULTICAST_DELEGATE_ThreeParams(FOnAreaBluffChanged, AAreaBase* AreaActor, const float BluffUnitAdd, const float BluffDefenseAdd);

UCLASS()
class STA_API AAreaBase : public AActor, public IInteractable, public IAbilitySystemInterface, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

public:
	AAreaBase();

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	void AttackedBy(AAreaBase* Attacker, const float AttackUnitNum);

	void AddLine(ALineBase* Line);

	TArray<AAreaBase*> GetConnectedArea();

	void SetHighlight(bool bIsHighlight);
	void SetAreaMaterialColor(FLinearColor Color);
	void SetTextRenderComponent();

	void SetLastScanTime();
	float GetLastScanTime() const { return LastScanTime; }

	FOnAreaValueChanged OnAreaValueChanged;
	FOnAreaBluffChanged OnAreaBluffChanged;

	/**
	* IInteractable
	*/
	virtual void OnHoverBegin() override;
	virtual void OnHoverEnd() override;
	virtual void OnInteractBegin(const FHitResult& HitResult) override;
	virtual void OnInteracting(const FHitResult& HitResult) override;
	virtual void OnInteractEnd(const FHitResult& HitResult) override;
	virtual const TArray<FInteractOption>& GetInteractOptions(FGenericTeamId Interactor) override;
	virtual FText GetInfoText(FGenericTeamId Interactor = FGenericTeamId::NoTeam) override;

protected:
	virtual void BeginPlay() override;

	void OnValueChanged(const FOnAttributeChangeData& Data);
	void OnBluffChanged(const FOnAttributeChangeData& Data);

	FText GetSimpleInfoText(FGenericTeamId Interactor);

	UPROPERTY(ReplicatedUsing = OnRep_OwningState)
	TWeakObjectPtr<APlayerState> OwningState = nullptr;

	UFUNCTION()
	void OnRep_OwningState();

	UPROPERTY(VisibleAnywhere, Category = "Component")
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
	
	UPROPERTY()
	TObjectPtr<UAreaAttributeSet> AttributeSet;

	UPROPERTY(VisibleAnywhere, Category = "Component")
	UStaticMeshComponent* AreaMesh;

	UPROPERTY(EditAnywhere, Category = "Sta|VFX")
	UNiagaraSystem* HighlightVFX;

	UPROPERTY(EditAnywhere, Category = "Sta|VFX")
	FVector HighlightSpawnLocation = FVector(0.0f, 0.0f, 30.0f);

	UPROPERTY(EditAnywhere, Category = "Component")
	UTextRenderComponent* TextRenderComponent;
	
	UPROPERTY(EditDefaultsOnly, Category = "Sta|GameplayEffect")
	TSubclassOf<UGameplayEffect> UnitChangeEffectClass;

private:
	void SetInteractOptions();

	UPROPERTY(EditAnywhere, Category = "Sta|VFX")
	UTeamPaletteData* TeamPaletteData;
	
	TArray<FInteractOption> FriendOptions;
	TArray<FInteractOption> HostileOptions;

	UPROPERTY()
	TArray<TObjectPtr<ALineBase>> ConnectLines;

	UPROPERTY(Transient)
	UNiagaraComponent* SpawnedHighlight;

	float LastScanTime = -1.0f;

public:
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

	/**
	 * Get/Set Func
	 */
	APlayerState* GetOwningState() const {return OwningState.Get();}

};
