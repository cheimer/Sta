// Sta Project - Multiplay RTS with GAS

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GenericTeamAgentInterface.h"
#include "InputTriggers.h"
#include "GameFramework/PlayerController.h"
#include "GameplayTag/StaTags.h"
#include "StaPlayerController.generated.h"


class AAreaBase;
struct FInteractOption;
struct FCardInfo;
class UCardData;
struct FGameplayEventData;
struct FGameplayTag;
class UInputMappingContext;

DECLARE_MULTICAST_DELEGATE_ThreeParams(FOnControllerStateChanged, FGameplayTag BeforeState, FGameplayTag AfterState, const TArray<FInteractOption>& NewOptions);
DECLARE_MULTICAST_DELEGATE(FOnControllerCanceled);

UENUM(BlueprintType)
enum class EInputPriority : uint8
{
	Character = 0,
	Controller = 1,

	Unknown UMETA(Hidden)
};

USTRUCT(BlueprintType)
struct FInputActionConfig
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> InputAction;
	
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	ETriggerEvent TriggerEvent = ETriggerEvent::Triggered;
	
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	FName FunctionName;
	
};


UCLASS()
class STA_API AStaPlayerController : public APlayerController, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

public:
	AStaPlayerController();

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;;

	void SetControllerIdle();
	void SetControllerTargeting();

	void TriggerGameplayEvent(FGameplayTag GameplayTag, const FGameplayEventData* EventData);
	
	UFUNCTION(Server, Reliable)
	void ServerTriggerGameplayEvent(FGameplayTag GameplayTag, const FGameplayEventData& EventData);

	UFUNCTION(Client, Reliable)
	void ClientDrawCard(const UCardData* DrawCardData);

	UFUNCTION(Client, Reliable)
	void ClientDiscardCard(const UCardData* RemoveCardData);

	UFUNCTION(Reliable, Client)
	void ClientAreaValueChanged(AAreaBase* AreaActor, const float UnitValue, const float DefenseValue);

	/**
	 * GenericTeamAgentInterface
	 */
	virtual void SetGenericTeamId(const FGenericTeamId& TeamID) override;
	virtual FGenericTeamId GetGenericTeamId() const override;
	/**
	 * ~GenericTeamAgentInterface
	 */

	FOnControllerStateChanged OnControllerStateChanged;
	FOnControllerCanceled OnControllerCanceled;

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	virtual void Tick(float DeltaTime) override;

	virtual void OnPossess(APawn* InPawn) override;

	void SetControllerState(FGameplayTag NewStateTag, const TArray<FInteractOption>& NewOptions = TArray<FInteractOption>());
	
	virtual void UpdateHoveredActor();

	UFUNCTION(Server, Reliable)
	void ServerInitDeck(const TArray<FCardInfo>& DeckList);

	virtual void OnRep_Pawn() override;
	virtual void OnRep_PlayerState() override;

#pragma region InputAction
	UFUNCTION()
	void InteractBegin(const FInputActionValue& Value);
	UFUNCTION()
	void Interacting(const FInputActionValue& Value);
	UFUNCTION()
	void InteractEnd(const FInputActionValue& Value);
	UFUNCTION()
	void Cancel(const FInputActionValue& Value);
	UFUNCTION()
	void Scroll(const FInputActionValue& Value);
	UFUNCTION()
	void Move(const FInputActionValue& Value);
	void EdgeScroll();
#pragma endregion InputAction
	
	UPROPERTY()
	TWeakObjectPtr<AActor> HoveredActor;
	
	bool bIsInteracting = false;

private:
	void TryBindingHUD();
	void TryInitDeckList();

	void SetConnectedAreasHighlight(AActor* RootArea, const bool bIsHighlight);

	UPROPERTY(EditDefaultsOnly, Category = "Sta|Input")
	UInputMappingContext* MappingContext;
	
	UPROPERTY(EditDefaultsOnly, Category = "Sta|Input")
	TArray<FInputActionConfig> InputActionConfigs;

	UPROPERTY(EditDefaultsOnly, Category = "Sta|Value")
	float EdgeSensitive = 50.0f;

	UPROPERTY(Transient)
	TWeakObjectPtr<AActor> RecentInteractActor;

	bool bHUDBounding = false;
	bool bInitDeckList = false;

	FGameplayTag StateTag = StaTags::State::Controller::Idle;

	UPROPERTY(Replicated)
	FGenericTeamId PlayerTeamID;
	
};
