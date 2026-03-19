// Sta Project - Multiplay RTS with GAS


#include "HTNTask_Wait.h"

bool UHTNTask_Wait::CheckPrecondition(const FHTNWorldState& WS) const
{
	return true;
}

void UHTNTask_Wait::SimulateEffect(FHTNWorldState& WS) const
{
	//
}

EHTNTaskStatus UHTNTask_Wait::Execute(AStaAIController* Owner, const FHTNWorldState& WS) const
{
	return EHTNTaskStatus::Success;
}
