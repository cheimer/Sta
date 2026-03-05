// Sta Project - Multiplay RTS with GAS

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Pawn.h"
#include "AIPawn.generated.h"

UCLASS()
class STA_API AAIPawn : public APawn, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AAIPawn();

protected:
	virtual void BeginPlay() override;

	virtual void PossessedBy(AController* NewController) override;

	/**
	 * GenericTeamAgentInterface
	 */
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	
};
