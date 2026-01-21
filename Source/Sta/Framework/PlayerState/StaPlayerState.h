// Sta Project - Multiplay RTS with GAS

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/PlayerState.h"
#include "StaPlayerState.generated.h"

struct FActiveGameplayEffectHandle;
class UGameplayEffect;
class UPlayerAttributeSet;
class UGameplayAbility;

/**
 * 
 */
UCLASS()
class STA_API AStaPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AStaPlayerState();

	void GiveDefaultAbilities();
	void ApplyDefaultEffects();

	/**
	 * AbilitySystemInterface
	 * @return ASC
	 */
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	
	UPlayerAttributeSet* GetAttributeSet() const;

protected:
	virtual void BeginPlay() override;
	
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
	
};
