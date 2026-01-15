// Sta Project - Multiplay RTS with GAS

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/PlayerState.h"
#include "StaPlayerState.generated.h"

class UCostComponent;
class UGameplayAbility;
class UAttributeSet;

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

	/**
	 * AbilitySystemInterface
	 * @return ASC
	 */
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	
	UAttributeSet* GetAttributeSet() const;

protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleAnywhere, Category = "Component")
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
	
	UPROPERTY(VisibleAnywhere, Category = "Component")
	TObjectPtr<UCostComponent> CostComponent;
	
	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;

	UPROPERTY(EditDefaultsOnly, Category = "Sta|Ability")
	TArray<TSubclassOf<UGameplayAbility>> DefaultAbilities;
	
};
