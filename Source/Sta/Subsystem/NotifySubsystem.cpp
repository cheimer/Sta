// Sta Project - Multiplay RTS with GAS


#include "NotifySubsystem.h"

#include "Area/AreaBase.h"
#include "Controller/StaPlayerController.h"

void UNotifySubsystem::NotifyAreaAttacked(ENotifyPriority Priority, FGenericTeamId AttackerTeam, const AAreaBase* AttackedArea, const float AttackUnitNum)
{
	if (GetWorld()->GetNetMode() != NM_DedicatedServer) return;
	
	AStaPlayerController* NotifyController = nullptr;
	FString AttackerName;
	
	for (FConstPlayerControllerIterator PCIter = GetWorld()->GetPlayerControllerIterator(); PCIter; ++PCIter)
	{
		AStaPlayerController* StaPlayerController = Cast<AStaPlayerController>(PCIter->Get());
		if (!StaPlayerController) continue;

		if (StaPlayerController->GetTeamId() == AttackedArea->GetGenericTeamId())
		{
			NotifyController = StaPlayerController;
		}
		else if (StaPlayerController->GetTeamId() == AttackerTeam)
		{
			AttackerName = StaPlayerController->GetName();
		}
	}
	if (!NotifyController) return;
	
	FText NotifyText = FText::FromString(FString::Printf(TEXT("%s is Attacked by \n%s : %.0f"), *AttackedArea->GetName(), *AttackerName, AttackUnitNum));
	NotifyController->NotifyToHUD(Priority, NotifyText);
}

void UNotifySubsystem::NotifyAreaAttackFail(ENotifyPriority Priority, FGenericTeamId AttackerTeam, const AAreaBase* AttackedArea, const float AttackUnitNum)
{
	if (GetWorld()->GetNetMode() != NM_DedicatedServer) return;
	
	AStaPlayerController* NotifyController = nullptr;
	FString AttackedName;
	
	for (FConstPlayerControllerIterator PCIter = GetWorld()->GetPlayerControllerIterator(); PCIter; ++PCIter)
	{
		AStaPlayerController* StaPlayerController = Cast<AStaPlayerController>(PCIter->Get());
		if (!StaPlayerController) continue;

		if (StaPlayerController->GetTeamId() == AttackerTeam)
		{
			NotifyController = StaPlayerController;
			break;
		}
	}
	if (!NotifyController) return;
	
	FText NotifyText = FText::FromString(FString::Printf(TEXT("%s Attack Fail\n Unit lost %.0f"), *AttackedArea->GetName(), AttackUnitNum));
	NotifyController->NotifyToHUD(Priority, NotifyText);
}

void UNotifySubsystem::NotifyAreaLost(ENotifyPriority Priority, FGenericTeamId LostTeam, const AAreaBase* LostArea)
{
	if (GetWorld()->GetNetMode() != NM_DedicatedServer) return;

	AStaPlayerController* NotifyController = nullptr;
	FString AttackerName;
	
	for (FConstPlayerControllerIterator PCIter = GetWorld()->GetPlayerControllerIterator(); PCIter; ++PCIter)
	{
		AStaPlayerController* StaPlayerController = Cast<AStaPlayerController>(PCIter->Get());
		if (!StaPlayerController) continue;

		if (StaPlayerController->GetTeamId() == LostTeam)
		{
			NotifyController = StaPlayerController;
		}
		else if (StaPlayerController->GetTeamId() == LostArea->GetGenericTeamId())
		{
			AttackerName = StaPlayerController->GetName();
		}
	}
	if (!NotifyController) return;
	
	FText NotifyText = FText::FromString(FString::Printf(TEXT("%s is Lost by %s"), *LostArea->GetName(), *AttackerName));
	NotifyController->NotifyToHUD(Priority, NotifyText);
}

void UNotifySubsystem::NotifyAreaCaptured(ENotifyPriority Priority, const AAreaBase* CapturedArea, const float AttackUnitNum, const float RemainUnitNum)
{
	if (GetWorld()->GetNetMode() != NM_DedicatedServer) return;

	AStaPlayerController* NotifyController = nullptr;
	
	for (FConstPlayerControllerIterator PCIter = GetWorld()->GetPlayerControllerIterator(); PCIter; ++PCIter)
	{
		AStaPlayerController* StaPlayerController = Cast<AStaPlayerController>(PCIter->Get());
		if (!StaPlayerController) continue;

		if (StaPlayerController->GetTeamId() == CapturedArea->GetGenericTeamId())
		{
			NotifyController = StaPlayerController;
			break;
		}
	}
	if (!NotifyController) return;
	
	FText NotifyText = FText::FromString(FString::Printf(TEXT("%s is Captured. \nAttack : %d -> Remain : %d"),
		*CapturedArea->GetName(), FMath::FloorToInt(AttackUnitNum), FMath::FloorToInt(RemainUnitNum)));
	NotifyController->NotifyToHUD(Priority, NotifyText);
}

void UNotifySubsystem::NotifyCostNeed(ENotifyPriority Priority, AController* Instigator)
{
	if (GetWorld()->GetNetMode() != NM_DedicatedServer) return;

	AStaPlayerController* NotifyController = Cast<AStaPlayerController>(Instigator);
	if (!NotifyController) return;
	
	FText NotifyText = FText::FromString(FString::Printf(TEXT("Need more Cost")));
	NotifyController->NotifyToHUD(Priority, NotifyText);
}
