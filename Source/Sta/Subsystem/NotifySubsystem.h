// Sta Project - Multiplay RTS with GAS

#pragma once

#include "CoreMinimal.h"
#include "GenericTeamAgentInterface.h"
#include "Subsystems/WorldSubsystem.h"
#include "NotifySubsystem.generated.h"

class AAreaBase;

UENUM()
enum class ENotifyPriority : uint8
{
	Low,
	Medium,
	High
};

/**
 * 
 */
UCLASS()
class STA_API UNotifySubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
	
public:
	void NotifyAreaAttacked(ENotifyPriority Priority, FGenericTeamId AttackerTeam, const AAreaBase* AttackedArea, const float AttackUnitNum);
	void NotifyAreaAttackFail(ENotifyPriority Priority, FGenericTeamId AttackerTeam, const AAreaBase* AttackedArea, const float AttackUnitNum);
	void NotifyAreaLost(ENotifyPriority Priority, FGenericTeamId LostTeam, const AAreaBase* LostArea);
	void NotifyAreaCaptured(ENotifyPriority Priority, const AAreaBase* CapturedArea, const float AttackUnitNum, const float RemainUnitNum);
	void NotifyCostNeed(ENotifyPriority Priority, AController* Instigator);

};
