// Sta Project - Multiplay RTS with GAS


#include "AreaInfoSubsystem.h"

#include "Area/AreaBase.h"
#include "Controller/StaPlayerController.h"
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
		}
	}
	
}

void UAreaInfoSubsystem::OnAreaValueChanged(AAreaBase* AreaActor, const float UnitValue, const float DefenseValue)
{
	if (!AreaActor) return;

	for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
	{
		IGenericTeamAgentInterface* TeamAgentInterface = Cast<IGenericTeamAgentInterface>(Iterator->Get());
		if (!TeamAgentInterface) continue;
		
		if (TeamAgentInterface->GetGenericTeamId() != AreaActor->GetGenericTeamId()) continue;
			
		if (AStaPlayerController* StaPlayerController = Cast<AStaPlayerController>(Iterator->Get()))
		{
			StaPlayerController->ClientAreaValueChanged(AreaActor, UnitValue, DefenseValue);
		}
		
	}
}
