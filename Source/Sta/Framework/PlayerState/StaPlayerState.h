// Sta Project - Multiplay RTS with GAS

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GenericTeamAgentInterface.h"
#include "GameFramework/PlayerState.h"
#include "StaPlayerState.generated.h"

struct FOnAttributeChangeData;
struct FActiveGameplayEffectHandle;
class UGameplayEffect;
class UPlayerAttributeSet;
class UGameplayAbility;

/**
 * 
 */
UCLASS()
class STA_API AStaPlayerState : public APlayerState, public IAbilitySystemInterface, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

public:
	AStaPlayerState();

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	void GiveDefaultAbilities();
	void ApplyDefaultEffects();

	/**
	 * AbilitySystemInterface
	 * @return ASC
	 */
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	
	UPlayerAttributeSet* GetAttributeSet() const;
	
	/**
	 * GenericTeamAgentInterface
	 */
	virtual void SetGenericTeamId(const FGenericTeamId& TeamID) override;
	virtual FGenericTeamId GetGenericTeamId() const override;

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void HandleChargeComplete();
	
	UPROPERTY(VisibleAnywhere, Category = "Component")
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
	
	UPROPERTY()
	TObjectPtr<UPlayerAttributeSet> AttributeSet;

	UPROPERTY(EditDefaultsOnly, Category = "Sta|Ability")
	TArray<TSubclassOf<UGameplayAbility>> DefaultAbilities;
	
	UPROPERTY(EditDefaultsOnly, Category = "Sta|Effect")
	TArray<TSubclassOf<UGameplayEffect>> DefaultEffects;

	UPROPERTY()
	TArray<FActiveGameplayEffectHandle> ActivatedEffectHandles;

private:
	UPROPERTY(Replicated)
	FGenericTeamId PlayerTeamID;

	float RecentChargeDrawTime = -1.0f;
	
};
