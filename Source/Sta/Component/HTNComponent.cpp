// Sta Project - Multiplay RTS with GAS


#include "HTNComponent.h"

#include "AbilitySystem/AttributeSet/AreaAttributeSet.h"
#include "AbilitySystem/AttributeSet/PlayerAttributeSet.h"
#include "AI/HTNTask/HTNTask_Attack.h"
#include "AI/HTNTask/HTNTask_DiscardCard.h"
#include "AI/HTNTask/HTNTask_DrawCard.h"
#include "AI/HTNTask/HTNTask_Employ.h"
#include "AI/HTNTask/HTNTask_Move.h"
#include "AI/HTNTask/HTNTask_Scan.h"
#include "AI/HTNTask/HTNTask_UseCard.h"
#include "AI/HTNTask/HTNTask_Wait.h"
#include "Area/AreaBase.h"
#include "Controller/StaAIController.h"
#include "DataAsset/AIStyleData.h"
#include "Framework/GameMode/StaGameModeBase.h"
#include "GameFramework/PlayerState.h"
#include "Helper/StaHelper.h"
#include "Kismet/GameplayStatics.h"


UHTNComponent::UHTNComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}

void UHTNComponent::BeginPlay()
{
	Super::BeginPlay();

	
}

void UHTNComponent::SetCurrentWorldState(const FHTNWorldState& NewState)
{
	CurrentWorldState = NewState;
}

bool UHTNComponent::RequestGeneratePlan(const UAIStyleData* StyleData)
{
	CurrentPlan.Empty();
	CurrentPlanIndex = 0;

	bool bSuccess = BuildPlan(CurrentWorldState, StyleData, CurrentPlan);

	if (bSuccess && CurrentPlan.Num() > 0)
	{
		PlannedWorldState = CurrentWorldState;
		bPlanValid = true;

		StaDebug::Print("Generate Plan");
		
		return true;
	}
	else
	{
		bPlanValid = false;
		return false;
	}

}

UHTNTaskBase* UHTNComponent::GetCurrentTask() const
{
	if (!HasValidPlan()) return nullptr;
	
	return CurrentPlan[CurrentPlanIndex];
}

void UHTNComponent::AdvancePlanIndex() 
{
	CurrentPlanIndex++;
	if (CurrentPlanIndex >= CurrentPlan.Num())
	{
		CompletePlan();
	}
}

void UHTNComponent::CompletePlan()
{
	CurrentPlan.Empty();
	CurrentPlanIndex = 0;
	bPlanValid = false;
}

void UHTNComponent::InvalidatePlan()
{
	bPlanValid = false;
}

bool UHTNComponent::HasValidPlan() const
{
	return bPlanValid && CurrentPlanIndex < CurrentPlan.Num();
}

float UHTNComponent::GetLastScanTime(AAreaBase* Area) const
{
	if (const float* Time = ScanHistory.Find(Area))
	{
		return *Time;
	}
	else
	{
		return -1.f;
	}
}

void UHTNComponent::RecordScan(AAreaBase* Area, float Time)
{
	ScanHistory.FindOrAdd(Area) = Time;
}

FHTNWorldState UHTNComponent::BuildWorldState(AStaAIController* Owner) const
{
	FHTNWorldState State;

	IGenericTeamAgentInterface* TeamAgent = Cast<IGenericTeamAgentInterface>(Owner->GetPlayerState<APlayerState>());
	if (!TeamAgent) return State;

	State.MyTeamId = TeamAgent->GetGenericTeamId();
	State.CurrentTime = Owner->GetWorld()->GetTimeSeconds();

	TArray<AActor*> AllAreaActors;
	UGameplayStatics::GetAllActorsOfClass(Owner->GetWorld(), AAreaBase::StaticClass(), AllAreaActors);

	TArray<AAreaBase*> AllAreas;
	for (AActor* Actor : AllAreaActors)
	{
		if (AAreaBase* Area = Cast<AAreaBase>(Actor))
		{
			AllAreas.Add(Area);
			State.Areas.Add(BuildAreaState(Area, State.MyTeamId, State.CurrentTime));
		}
	}

	BuildConnection(AllAreas, State);
	CollectResourceState(Owner, State);

	return State;
}

FHTNAreaState UHTNComponent::BuildAreaState(AAreaBase* Area, FGenericTeamId MyTeamId, float CurrentTime) const
{
	FHTNAreaState AreaState;
	AreaState.AreaActor = Area;
	AreaState.OwningTeam = Area->GetGenericTeamId();

	UAreaAttributeSet* Attr = Area->GetAttributeSet();
	if (!Attr) return AreaState;

	if (AreaState.OwningTeam == MyTeamId)
	{
		AreaState.UnitNum = Attr->GetUnitNum();
		AreaState.Defense = Attr->GetDefense();
		AreaState.BluffUnitAdd = Attr->GetBluffUnitAdd();
		AreaState.BluffDefenseAdd = Attr->GetBluffDefenseAdd();
		AreaState.LastScanTime = CurrentTime;
	}
	else
	{
		AreaState.LastScanTime = GetLastScanTime(Area);
		if (AreaState.LastScanTime < 0)
		{
			AreaState.UnitNum = Attr->GetUnitNum() + Attr->GetBluffUnitAdd();
			AreaState.Defense = Attr->GetDefense() + Attr->GetBluffDefenseAdd();
			AreaState.BluffUnitAdd = 0.f;
			AreaState.BluffDefenseAdd = 0.f;
		}
		else
		{
			AreaState.UnitNum = Attr->GetUnitNum();
			AreaState.Defense = Attr->GetDefense();
			AreaState.BluffUnitAdd = Attr->GetBluffUnitAdd();
			AreaState.BluffDefenseAdd = Attr->GetBluffDefenseAdd();
		}
	}

	return AreaState;
}

void UHTNComponent::BuildConnection(const TArray<AAreaBase*>& AllAreas, FHTNWorldState& OutState) const
{
	TMap<AAreaBase*, int32> IndexMap;
	for (int32 i = 0; i < AllAreas.Num(); i++)
	{
		IndexMap.Add(AllAreas[i], i);
	}

	for (int32 i = 0; i < AllAreas.Num(); i++)
	{
		for (AAreaBase* Neighbor : AllAreas[i]->GetConnectedArea())
		{
			if (int32* Idx = IndexMap.Find(Neighbor))
			{
				OutState.Areas[i].ConnectedAreaIndices.Add(*Idx);
			}
		}
	}
}

void UHTNComponent::CollectResourceState(AStaAIController* Owner, FHTNWorldState& OutState) const
{
	if (!Owner) return;
	
	AStaGameModeBase* GameMode = Cast<AStaGameModeBase>(Owner->GetWorld()->GetAuthGameMode());
	IAbilitySystemInterface* ASCInterface = Cast<IAbilitySystemInterface>(Owner->GetPlayerState<APlayerState>());
	if (!GameMode || !ASCInterface) return;
	
	UAbilitySystemComponent* ASC = ASCInterface->GetAbilitySystemComponent();
	if (!ASC) return;
	
	OutState.CurrentCost = ASC->GetNumericAttribute(UPlayerAttributeSet::GetCostAttribute());
	OutState.MaxCost = ASC->GetNumericAttribute(UPlayerAttributeSet::GetMaxCostAttribute());
	
	for (const UCardData* Card : GameMode->GetHandCards(Owner))
	{
		if (!Card) continue;
		
		FHTNCardState CardState;
		CardState.CardData = Card;
		CardState.Cost = Card->Cost;
		CardState.CardTag = Card->CardTag;
		
		OutState.HandCards.Add(CardState);
	}
	
	OutState.bCanDraw = GameMode->CanDrawCard(Owner);
	
}

int32 UHTNComponent::FindCardByTag(const FHTNWorldState& WS, const FGameplayTag& Tag) const
{
	for (int32 i = 0; i < WS.HandCards.Num(); i++)
	{
		if (WS.HandCards[i].CardTag.MatchesTag(Tag)
			&& WS.CurrentCost >= WS.HandCards[i].Cost)
		{
			return i;
		}
	}
	return INDEX_NONE;
}

int32 UHTNComponent::FindWorstCard(const FHTNWorldState& WS, const UAIStyleData* Style) const
{
	float WorstScore = TNumericLimits<float>::Max();
	int32 WorstIdx = INDEX_NONE;

	for (int32 i = 0; i < WS.HandCards.Num(); i++)
	{
		float Score = 0.f;
		const FGameplayTag& Tag = WS.HandCards[i].CardTag;

		if (Tag.MatchesTag(StaTags::Ability::Card::Spell))
		{
			Score = Style->Aggressive;
		}
		else if (Tag.MatchesTag(StaTags::Ability::Card::Employ))
		{
			Score = 0.5f;
		}
		else if (Tag.MatchesTag(StaTags::Ability::Card::Bluff))
		{
			Score = Style->Tricky;
		}

		if (Score < WorstScore)
		{
			WorstScore = Score;
			WorstIdx = i;
		}
	}

	return WorstIdx;
}
bool UHTNComponent::BuildPlan(const FHTNWorldState& State, const UAIStyleData* Style, TArray<UHTNTaskBase*>& OutPlan)
{
	FHTNWorldState WS = State;
	OutPlan.Empty();

	if (TryEmergencyDefend(WS, Style, OutPlan)) return true;
	if (TryCapture(WS, Style, OutPlan)) return true;
	if (TryStrengthen(WS, Style, OutPlan)) return true;
	if (TryWaitOneTime(WS, Style, OutPlan)) return true;
	
	return false;
}

bool UHTNComponent::TryEmergencyDefend(FHTNWorldState& WS, const UAIStyleData* Style, TArray<UHTNTaskBase*>& OutPlan)
{
	int32 ThreatenedIdx = FindMostThreatenedArea(WS, Style);
	if (ThreatenedIdx == INDEX_NONE) return false;

	if (TryDefendWithCard(WS, Style, ThreatenedIdx, OutPlan)) return true;
	if (TryDefendWithReinforce(WS, Style, ThreatenedIdx, OutPlan)) return true;
	if (TryDefendWithEmploy(WS, Style, ThreatenedIdx, OutPlan)) return true;
	
	return false;
}

bool UHTNComponent::TryDefendWithCard(FHTNWorldState& WS, const UAIStyleData* Style, int32 AreaIdx, TArray<UHTNTaskBase*>& OutPlan)
{
	int32 CardIdx = FindCardByTag(WS, StaTags::Ability::Card::Employ);
	if (CardIdx == INDEX_NONE) return false;

	FHTNWorldState Backup = WS;
	TArray<UHTNTaskBase*> BackupPlan = OutPlan;

	UHTNTaskBase* CardTask = CreateUseCardTask(CardIdx, AreaIdx);
	if (!CardTask->CheckPrecondition(WS))
	{
		WS = Backup; OutPlan = BackupPlan;
		return false;
	}
	CardTask->SimulateEffect(WS);
	OutPlan.Add(CardTask);

	return true;
}

bool UHTNComponent::TryDefendWithReinforce(FHTNWorldState& WS, const UAIStyleData* Style, int32 AreaIdx, TArray<UHTNTaskBase*>& OutPlan)
{
	const FHTNAreaState& Threatened = WS.Areas[AreaIdx];

	int32 BestSupport = INDEX_NONE;
	float MaxUnits = 0.f;

	for (int32 Neighbor : Threatened.ConnectedAreaIndices)
	{
		if (WS.Areas[Neighbor].OwningTeam != WS.MyTeamId) continue;
		if (WS.Areas[Neighbor].UnitNum > MaxUnits)
		{
			MaxUnits = WS.Areas[Neighbor].UnitNum;
			BestSupport = Neighbor;
		}
	}

	if (BestSupport == INDEX_NONE || MaxUnits <= 1.f) return false;

	float MoveUnits = MaxUnits - 1.f;

	FHTNWorldState Backup = WS;
	TArray<UHTNTaskBase*> BackupPlan = OutPlan;

	UHTNTaskBase* MoveTask = CreateMoveTask(BestSupport, AreaIdx, MoveUnits);
	if (!MoveTask->CheckPrecondition(WS))
	{
		WS = Backup; OutPlan = BackupPlan;
		return false;
	}
	MoveTask->SimulateEffect(WS);
	OutPlan.Add(MoveTask);

	return true;
}

bool UHTNComponent::TryDefendWithEmploy(FHTNWorldState& WS, const UAIStyleData* Style, int32 AreaIdx, TArray<UHTNTaskBase*>& OutPlan)
{
	FHTNWorldState Backup = WS;
	TArray<UHTNTaskBase*> BackupPlan = OutPlan;

	UHTNTaskBase* EmployTask = CreateEmployTask(AreaIdx);
	if (!EmployTask->CheckPrecondition(WS))
	{
		WS = Backup; OutPlan = BackupPlan;
		return false;
	}
	EmployTask->SimulateEffect(WS);
	OutPlan.Add(EmployTask);

	return true;
}

bool UHTNComponent::TryCapture(FHTNWorldState& WS, const UAIStyleData* Style, TArray<UHTNTaskBase*>& OutPlan)
{
	int32 TargetIdx = FindBestAttackTarget(WS, Style);
	if (TargetIdx == INDEX_NONE) return false;

	if (NeedScan(WS, Style, TargetIdx))
	{
		OutPlan.Add(CreateScanTask(TargetIdx));
		return true;
	}

	if (TryCaptureDirectAttack(WS, Style, TargetIdx, OutPlan)) return true;
	if (TryCaptureWithReinforce(WS, Style, TargetIdx, OutPlan)) return true;
	if (TryCaptureWithCard(WS, Style, TargetIdx, OutPlan)) return true;

	return false;
}

bool UHTNComponent::TryCaptureDirectAttack(FHTNWorldState& WS, const UAIStyleData* Style, int32 TargetIdx, TArray<UHTNTaskBase*>& OutPlan)
{
	const FHTNAreaState& Target = WS.Areas[TargetIdx];

	int32 BestSrc = INDEX_NONE;
	float MaxUnits = 0.f;
	for (int32 Neighbor : Target.ConnectedAreaIndices)
	{
		if (WS.Areas[Neighbor].OwningTeam != WS.MyTeamId) continue;
		if (WS.Areas[Neighbor].UnitNum > MaxUnits)
		{
			MaxUnits = WS.Areas[Neighbor].UnitNum;
			BestSrc = Neighbor;
		}
	}
	if (BestSrc == INDEX_NONE) return false;

	float AttackUnits = MaxUnits - 1.f;
	if (AttackUnits <= 0.f) return false;

	FHTNWorldState Backup = WS;
	TArray<UHTNTaskBase*> BackupPlan = OutPlan;

	UHTNTaskBase* AttackTask = CreateAttackTask(BestSrc, TargetIdx, AttackUnits);
	if (!AttackTask->CheckPrecondition(WS))
	{
		WS = Backup; OutPlan = BackupPlan;
		return false;
	}
	AttackTask->SimulateEffect(WS);
	OutPlan.Add(AttackTask);

	if (WS.Areas[TargetIdx].OwningTeam != WS.MyTeamId)
	{
		WS = Backup; OutPlan = BackupPlan;
		return false;
	}

	return true;
}

bool UHTNComponent::TryCaptureWithReinforce(FHTNWorldState& WS, const UAIStyleData* Style, int32 TargetIdx, TArray<UHTNTaskBase*>& OutPlan)
{
	TArray<int32> RearAreas = WS.GetRearAreas();
	if (RearAreas.IsEmpty()) return false;

	int32 FrontIdx = INDEX_NONE;
	for (int32 Neighbor : WS.Areas[TargetIdx].ConnectedAreaIndices)
	{
		if (WS.Areas[Neighbor].OwningTeam == WS.MyTeamId)
		{
			FrontIdx = Neighbor;
			break;
		}
	}
	if (FrontIdx == INDEX_NONE) return false;

	int32 BestRear = INDEX_NONE;
	float MaxUnits = 0.f;
	for (int32 RearIdx : RearAreas)
	{
		if (!WS.Areas[RearIdx].ConnectedAreaIndices.Contains(FrontIdx))
			continue;
		if (WS.Areas[RearIdx].UnitNum > MaxUnits)
		{
			MaxUnits = WS.Areas[RearIdx].UnitNum;
			BestRear = RearIdx;
		}
	}
	if (BestRear == INDEX_NONE || MaxUnits <= 1.f) return false;

	float MoveUnits = MaxUnits - 1.f;

	FHTNWorldState Backup = WS;
	TArray<UHTNTaskBase*> BackupPlan = OutPlan;

	UHTNTaskBase* MoveTask = CreateMoveTask(BestRear, FrontIdx, MoveUnits);
	if (!MoveTask->CheckPrecondition(WS))
	{
		WS = Backup; OutPlan = BackupPlan;
		return false;
	}
	MoveTask->SimulateEffect(WS);
	OutPlan.Add(MoveTask);

	float AttackUnits = WS.Areas[FrontIdx].UnitNum - 1.f;
	UHTNTaskBase* AttackTask = CreateAttackTask(FrontIdx, TargetIdx, AttackUnits);
	if (!AttackTask->CheckPrecondition(WS))
	{
		WS = Backup; OutPlan = BackupPlan;
		return false;
	}
	AttackTask->SimulateEffect(WS);
	OutPlan.Add(AttackTask);

	if (WS.Areas[TargetIdx].OwningTeam != WS.MyTeamId)
	{
		WS = Backup; OutPlan = BackupPlan;
		return false;
	}

	return true;
}

bool UHTNComponent::TryCaptureWithCard(FHTNWorldState& WS, const UAIStyleData* Style, int32 TargetIdx, TArray<UHTNTaskBase*>& OutPlan)
{
	int32 CardIdx = FindCardByTag(WS, StaTags::Ability::Card::Spell);
	if (CardIdx == INDEX_NONE) return false;

	int32 BestSrc = INDEX_NONE;
	float MaxUnits = 0.f;
	for (int32 Neighbor : WS.Areas[TargetIdx].ConnectedAreaIndices)
	{
		if (WS.Areas[Neighbor].OwningTeam != WS.MyTeamId) continue;
		if (WS.Areas[Neighbor].UnitNum > MaxUnits)
		{
			MaxUnits = WS.Areas[Neighbor].UnitNum;
			BestSrc = Neighbor;
		}
	}
	if (BestSrc == INDEX_NONE) return false;

	FHTNWorldState Backup = WS;
	TArray<UHTNTaskBase*> BackupPlan = OutPlan;

	UHTNTaskBase* CardTask = CreateUseCardTask(CardIdx, TargetIdx);
	if (!CardTask->CheckPrecondition(WS))
	{
		WS = Backup;
		OutPlan = BackupPlan;
		return false;
	}
	CardTask->SimulateEffect(WS);
	OutPlan.Add(CardTask);

	float AttackUnits = WS.Areas[BestSrc].UnitNum - 1.f;
	if (AttackUnits <= 0.f)
	{
		WS = Backup;
		OutPlan = BackupPlan;
		return false;
	}

	UHTNTaskBase* AttackTask = CreateAttackTask(BestSrc, TargetIdx, AttackUnits);
	if (!AttackTask->CheckPrecondition(WS))
	{
		WS = Backup;
		OutPlan = BackupPlan;
		return false;
	}
	AttackTask->SimulateEffect(WS);
	OutPlan.Add(AttackTask);

	if (WS.Areas[TargetIdx].OwningTeam != WS.MyTeamId)
	{
		WS = Backup;
		OutPlan = BackupPlan;
		return false;
	}

	return true;
}

bool UHTNComponent::TryStrengthen(FHTNWorldState& WS, const UAIStyleData* Style, TArray<UHTNTaskBase*>& OutPlan)
{
	if (TryBluff(WS, Style, OutPlan)) return true;
	if (TryReinforce(WS, Style, OutPlan)) return true;
	if (TryManageCards(WS, Style, OutPlan)) return true;
	if (TryPreemptiveScan(WS, Style, OutPlan)) return true;
	if (TryStrengthenWithEmploy(WS, Style, OutPlan)) return true;

	return false;
}

bool UHTNComponent::TryWaitOneTime(FHTNWorldState& WS, const UAIStyleData* Style, TArray<UHTNTaskBase*>& OutPlan)
{
	FHTNWorldState Backup = WS;
	TArray<UHTNTaskBase*> BackupPlan = OutPlan;

	UHTNTaskBase* WaitTask = CreateWaitTask();
	
	if (!WaitTask->CheckPrecondition(WS))
	{
		WS = Backup;
		OutPlan = BackupPlan;
		return false;
	}
	
	WaitTask->SimulateEffect(WS);
	OutPlan.Add(WaitTask);

	return true;
}

bool UHTNComponent::TryBluff(FHTNWorldState& WS, const UAIStyleData* Style, TArray<UHTNTaskBase*>& OutPlan)
{
	if (Style->Tricky < 0.5f) return false;

	int32 CardIdx = FindCardByTag(WS, StaTags::Ability::Card::Bluff);
	if (CardIdx == INDEX_NONE) return false;

	int32 WeakestFront = INDEX_NONE;
	float MinUnits = TNumericLimits<float>::Max();

	for (int32 FrontIdx : WS.GetFrontlineAreas())
	{
		if (WS.Areas[FrontIdx].UnitNum < MinUnits)
		{
			MinUnits = WS.Areas[FrontIdx].UnitNum;
			WeakestFront = FrontIdx;
		}
	}
	if (WeakestFront == INDEX_NONE) return false;

	FHTNWorldState Backup = WS;
	TArray<UHTNTaskBase*> BackupPlan = OutPlan;

	UHTNTaskBase* BluffTask = CreateUseCardTask(CardIdx, WeakestFront);
	if (!BluffTask->CheckPrecondition(WS))
	{
		WS = Backup; OutPlan = BackupPlan;
		return false;
	}
	BluffTask->SimulateEffect(WS);
	OutPlan.Add(BluffTask);

	return true;
}

bool UHTNComponent::TryReinforce(FHTNWorldState& WS, const UAIStyleData* Style, TArray<UHTNTaskBase*>& OutPlan)
{
	TArray<int32> RearAreas = WS.GetRearAreas();
	TArray<int32> FrontAreas = WS.GetFrontlineAreas();
	if (RearAreas.IsEmpty() || FrontAreas.IsEmpty()) return false;

	int32 BestRear = INDEX_NONE;
	float MaxRearUnits = 0.f;
	for (int32 RearIdx : RearAreas)
	{
		if (WS.Areas[RearIdx].UnitNum > MaxRearUnits)
		{
			MaxRearUnits = WS.Areas[RearIdx].UnitNum;
			BestRear = RearIdx;
		}
	}
	if (BestRear == INDEX_NONE || MaxRearUnits <= 1.f) return false;

	int32 WeakestFront = INDEX_NONE;
	float MinFrontUnits = TNumericLimits<float>::Max();
	for (int32 FrontIdx : FrontAreas)
	{
		if (!WS.Areas[BestRear].ConnectedAreaIndices.Contains(FrontIdx))
			continue;
		if (WS.Areas[FrontIdx].UnitNum < MinFrontUnits)
		{
			MinFrontUnits = WS.Areas[FrontIdx].UnitNum;
			WeakestFront = FrontIdx;
		}
	}
	if (WeakestFront == INDEX_NONE) return false;

	float MoveUnits = MaxRearUnits - 1.f;

	FHTNWorldState Backup = WS;
	TArray<UHTNTaskBase*> BackupPlan = OutPlan;

	UHTNTaskBase* MoveTask = CreateMoveTask(BestRear, WeakestFront, MoveUnits);
	if (!MoveTask->CheckPrecondition(WS))
	{
		WS = Backup; OutPlan = BackupPlan;
		return false;
	}
	MoveTask->SimulateEffect(WS);
	OutPlan.Add(MoveTask);

	return true;
}

bool UHTNComponent::TryManageCards(FHTNWorldState& WS, const UAIStyleData* Style, TArray<UHTNTaskBase*>& OutPlan)
{
	if (WS.HandCards.Num() >= WS.MaxHandSize)
	{
		int32 WorstIdx = FindWorstCard(WS, Style);
		if (WorstIdx != INDEX_NONE)
		{
			OutPlan.Add(CreateDiscardCardTask(WorstIdx));
			return true;
		}
	}

	if (WS.bCanDraw
		&& WS.HandCards.Num() < WS.MaxHandSize
		&& WS.CurrentCost >= DrawCost)
	{
		UHTNTaskBase* DrawTask = CreateDrawCardTask();
		if (DrawTask->CheckPrecondition(WS))
		{
			DrawTask->SimulateEffect(WS);
			OutPlan.Add(DrawTask);
			return true;
		}
	}

	return false;
}

bool UHTNComponent::TryPreemptiveScan(FHTNWorldState& WS, const UAIStyleData* Style, TArray<UHTNTaskBase*>& OutPlan)
{
	if (WS.CurrentCost < ScanCost) return false;

	float OldestScan = TNumericLimits<float>::Max();
	int32 OldestIdx = INDEX_NONE;

	for (int32 FrontIdx : WS.GetFrontlineAreas())
	{
		for (int32 Neighbor : WS.Areas[FrontIdx].ConnectedAreaIndices)
		{
			if (WS.Areas[Neighbor].OwningTeam == WS.MyTeamId) continue;
			if (WS.Areas[Neighbor].OwningTeam == FGenericTeamId::NoTeam) continue;

			if (!NeedScan(WS, Style, Neighbor)) continue;

			float ScanTime = WS.Areas[Neighbor].LastScanTime;
			if (ScanTime < 0) ScanTime = -9999.f;

			if (ScanTime < OldestScan)
			{
				OldestScan = ScanTime;
				OldestIdx = Neighbor;
			}
		}
	}

	if (OldestIdx == INDEX_NONE) return false;

	OutPlan.Add(CreateScanTask(OldestIdx));
	return true;
}

bool UHTNComponent::TryStrengthenWithEmploy(FHTNWorldState& WS, const UAIStyleData* Style, TArray<UHTNTaskBase*>& OutPlan)
{
	if (WS.CurrentCost < EmployCost) return false;

	TArray<int32> FrontAreas = WS.GetFrontlineAreas();
	if (FrontAreas.IsEmpty()) return false;

	int32 WeakestFront = INDEX_NONE;
	float MinUnits = TNumericLimits<float>::Max();
	for (int32 FrontIdx : FrontAreas)
	{
		if (WS.Areas[FrontIdx].UnitNum < MinUnits)
		{
			MinUnits = WS.Areas[FrontIdx].UnitNum;
			WeakestFront = FrontIdx;
		}
	}
	if (WeakestFront == INDEX_NONE) return false;

	FHTNWorldState Backup = WS;
	TArray<UHTNTaskBase*> BackupPlan = OutPlan;

	UHTNTaskBase* EmployTask = CreateEmployTask(WeakestFront);
	if (!EmployTask->CheckPrecondition(WS))
	{
		WS = Backup; OutPlan = BackupPlan;
		return false;
	}
	EmployTask->SimulateEffect(WS);
	OutPlan.Add(EmployTask);

	return true;
}

int32 UHTNComponent::FindBestAttackTarget(const FHTNWorldState& WS, const UAIStyleData* Style) const
{
	float BestScore = -TNumericLimits<float>::Min();
	int32 BestTarget = INDEX_NONE;

	float Threshold = 1.5f - Style->Aggressive;

	for (int32 EnemyIdx : WS.GetEnemyAreas())
	{
		const FHTNAreaState& EnemyArea = WS.Areas[EnemyIdx];
		float EnemyDefenseValue = UAreaCalc::CalcGetDefenseValue(EnemyArea.GetAreaUnitNum(WS.MyTeamId), EnemyArea.GetAreaDefense(WS.MyTeamId));

		for (int32 Neighbor : EnemyArea.ConnectedAreaIndices)
		{
			if (WS.Areas[Neighbor].OwningTeam != WS.MyTeamId) continue;

			float AvailableUnits = WS.Areas[Neighbor].UnitNum - 1.f;
			if (AvailableUnits <= 0.f) continue;

			float Ratio = EnemyDefenseValue > 0.f ? AvailableUnits / EnemyDefenseValue : 999.f;

			if (Ratio >= Threshold && Ratio > BestScore)
			{
				BestScore = Ratio;
				BestTarget = EnemyIdx;
			}
		}
	}

	return BestTarget;
}

int32 UHTNComponent::FindMostThreatenedArea(const FHTNWorldState& WS, const UAIStyleData* Style) const
{
	float HighestThreat = 0.f;
	int32 MostThreatened = INDEX_NONE;

	float Threshold = 1.0f - Style->Aggressive;

	for (int32 MyIdx : WS.GetFrontlineAreas())
	{
		float Threat = CalcThreatLevel(WS, MyIdx);
		if (Threat > Threshold && Threat > HighestThreat)
		{
			HighestThreat = Threat;
			MostThreatened = MyIdx;
		}
	}

	return MostThreatened;
}

bool UHTNComponent::NeedScan(const FHTNWorldState& WS, const UAIStyleData* Style, int32 AreaIdx) const
{
	const FHTNAreaState& Area = WS.Areas[AreaIdx];
	if (Area.OwningTeam == WS.MyTeamId) return false;

	if (WS.CurrentCost < ScanCost) return false;

	if (Area.LastScanTime < 0) return true;

	float ScanInterval = 30.f - Style->Careful * 20.f;
	float ElapsedSinceLastScan = WS.CurrentTime - Area.LastScanTime;

	return ElapsedSinceLastScan > ScanInterval;
}

float UHTNComponent::CalcThreatLevel(const FHTNWorldState& WS, int32 AreaIdx) const
{
	const FHTNAreaState& MyArea = WS.Areas[AreaIdx];
	float MyDefenseValue = UAreaCalc::CalcGetDefenseValue(MyArea.UnitNum, MyArea.Defense);

	float TotalEnemyAttack = 0.f;
	for (int32 Neighbor : MyArea.ConnectedAreaIndices)
	{
		const FHTNAreaState& NeighborArea = WS.Areas[Neighbor];
		if (NeighborArea.OwningTeam == WS.MyTeamId || NeighborArea.OwningTeam == FGenericTeamId::NoTeam) continue;

		float EnemyAttack = FMath::Max(0.f,NeighborArea.GetAreaUnitNum(WS.MyTeamId));
		TotalEnemyAttack += EnemyAttack;
	}

	if (TotalEnemyAttack <= 0.f) return 0.f;

	return TotalEnemyAttack / MyDefenseValue;
}

UHTNTaskBase* UHTNComponent::CreateAttackTask(int32 Src, int32 Dest, float Units)
{
	UHTNTask_Attack* Task = NewObject<UHTNTask_Attack>(this);
	Task->SourceAreaIndex = Src;
	Task->TargetAreaIndex = Dest;
	Task->UnitCount = Units;
	
	return Task;
}

UHTNTaskBase* UHTNComponent::CreateMoveTask(int32 Src, int32 Dest, float Units)
{
	UHTNTask_Move* Task = NewObject<UHTNTask_Move>(this);
	Task->SourceAreaIndex = Src;
	Task->TargetAreaIndex = Dest;
	Task->UnitCount = Units;
	
	return Task;
}

UHTNTaskBase* UHTNComponent::CreateScanTask(int32 Target)
{
	UHTNTask_Scan* Task = NewObject<UHTNTask_Scan>(this);
	Task->TargetAreaIndex = Target;
	Task->ScanCost = ScanCost;
	
	return Task;
}

UHTNTaskBase* UHTNComponent::CreateEmployTask(int32 Target)
{
	UHTNTask_Employ* Task = NewObject<UHTNTask_Employ>(this);
	Task->TargetAreaIndex = Target;
	Task->EmployCost = EmployCost;
	Task->EmployNum = EmployNum;
	
	return Task;
}

UHTNTaskBase* UHTNComponent::CreateUseCardTask(int32 CardIdx, int32 Target)
{
	UHTNTask_UseCard* Task = NewObject<UHTNTask_UseCard>(this);
	Task->CardIndex = CardIdx;
	Task->TargetAreaIndex = Target;
	
	return Task;
}

UHTNTaskBase* UHTNComponent::CreateDrawCardTask()
{
	UHTNTask_DrawCard* Task = NewObject<UHTNTask_DrawCard>(this);
	Task->DrawCost = DrawCost;
	
	return Task;
}

UHTNTaskBase* UHTNComponent::CreateDiscardCardTask(int32 CardIdx)
{
	UHTNTask_DiscardCard* Task = NewObject<UHTNTask_DiscardCard>(this);
	Task->CardIndex = CardIdx;
	
	return Task;
}

UHTNTaskBase* UHTNComponent::CreateWaitTask()
{
	UHTNTask_Wait* Task = NewObject<UHTNTask_Wait>(this);
	
	return Task;
}
