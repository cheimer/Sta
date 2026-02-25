// Sta Project - Multiplay RTS with GAS


#include "AreaInfoSubsystem.h"

#include "Area/AreaBase.h"
#include "Controller/StaPlayerController.h"
#include "FunctionLibrary/AreaCalc.h"
#include "GameFramework/PlayerState.h"
#include "Kismet/GameplayStatics.h"

void UAreaInfoSubsystem::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);
	
	if (GetWorld()->GetNetMode() == NM_Client) return;
	
	TArray<AActor*> AllAreaActors;
	UGameplayStatics::GetAllActorsOfClass(InWorld.GetWorld(), AAreaBase::StaticClass(), AllAreaActors);

	for (AActor* AreaActor : AllAreaActors)
	{
		if (AAreaBase* Area = Cast<AAreaBase>(AreaActor))
		{
			Area->OnAreaValueChanged.AddUObject(this, &ThisClass::OnAreaValueChanged);
			Area->OnAreaBluffChanged.AddUObject(this, &ThisClass::OnAreaBluffChanged);
		}
	}
	
}

void UAreaInfoSubsystem::OnAreaValueChanged(AAreaBase* AreaActor, const float UnitValue, const float DefenseValue)
{
	if (!AreaActor || !AreaActor->GetOwningState()) return;

	IGenericTeamAgentInterface* OwningTeam = Cast<IGenericTeamAgentInterface>(AreaActor->GetOwningState());
	if (!OwningTeam) return;

	if (OwningTeam->GetGenericTeamId() != AreaActor->GetGenericTeamId()) return;

	if (AStaPlayerController* StaPlayerController = Cast<AStaPlayerController>(AreaActor->GetOwningState()->GetOwningController()))
	{
		StaPlayerController->ClientAreaValueChanged(AreaActor, UnitValue, DefenseValue);
	}
	
}

void UAreaInfoSubsystem::OnAreaBluffChanged(AAreaBase* AreaActor, const float BluffUnitAdd, const float BluffDefenseAdd)
{
	if (!AreaActor || !AreaActor->GetOwningState()) return;

	IGenericTeamAgentInterface* OwningTeam = Cast<IGenericTeamAgentInterface>(AreaActor->GetOwningState());
	if (!OwningTeam) return;

	if (OwningTeam->GetGenericTeamId() != AreaActor->GetGenericTeamId()) return;

	if (AStaPlayerController* StaPlayerController = Cast<AStaPlayerController>(AreaActor->GetOwningState()->GetOwningController()))
	{
		StaPlayerController->ClientAreaBluffChanged(AreaActor, BluffUnitAdd, BluffDefenseAdd);
	}
	
}

void UAreaInfoSubsystem::UpdateAreaBluffTo(AAreaBase* TargetArea, APlayerState* PS)
{
	if (!TargetArea || !TargetArea->GetOwningState() || !PS) return;

	if (AStaPlayerController* StaPlayerController = Cast<AStaPlayerController>(PS->GetOwningController()))
	{
		StaPlayerController->ClientAreaValueChanged(TargetArea, UAreaCalc::CalcBluffUnit(TargetArea), UAreaCalc::CalcBluffDefense(TargetArea));
	}
}
