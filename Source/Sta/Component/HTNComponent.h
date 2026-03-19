// Sta Project - Multiplay RTS with GAS

#pragma once

#include "CoreMinimal.h"
#include "HTNTypes.h"
#include "Components/ActorComponent.h"
#include "HTNComponent.generated.h"

class AStaAIController;
class UHTNTaskBase;
class UAIStyleData;
struct FHTNWorldState;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class STA_API UHTNComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UHTNComponent();

	void SetCurrentWorldState(const FHTNWorldState& NewState);
	const FHTNWorldState& GetCurrentWorldState() const { return CurrentWorldState; }
	const FHTNWorldState& GetPlannedWorldState() const { return PlannedWorldState; }

	bool RequestGeneratePlan(const UAIStyleData* StyleData);

	UHTNTaskBase* GetCurrentTask() const;
	void AdvancePlanIndex();
	void CompletePlan();
	void InvalidatePlan();

	bool HasValidPlan() const;
	int32 GetCurrentPlanIndex() const { return CurrentPlanIndex; }

	FHTNWorldState BuildWorldState(AStaAIController* Owner) const;

	float GetLastScanTime(AAreaBase* Area) const;
	void RecordScan(AAreaBase* Area, float Time);

	UPROPERTY(EditAnywhere, Category = "Sta|Value")
	float ScanCost = 1.0f;

	UPROPERTY(EditAnywhere, Category = "Sta|Value")
	float DrawCost = 1.0f;

	UPROPERTY(EditAnywhere, Category = "Sta|Value")
	float EmployCost = 2.0f;

	UPROPERTY(EditAnywhere, Category = "Sta|Value")
	float EmployNum = 5.0f;

protected:
	virtual void BeginPlay() override;
	
private:
	int32 FindCardByTag(const FHTNWorldState& WS, const FGameplayTag& Tag) const;
	int32 FindWorstCard(const FHTNWorldState& WS, const UAIStyleData* Style) const;
	
    bool BuildPlan(const FHTNWorldState& State, const UAIStyleData* Style, TArray<UHTNTaskBase*>& OutPlan);

    bool TryEmergencyDefend(FHTNWorldState& WS, const UAIStyleData* Style, TArray<UHTNTaskBase*>& OutPlan);
    bool TryCapture(FHTNWorldState& WS, const UAIStyleData* Style, TArray<UHTNTaskBase*>& OutPlan);
    bool TryStrengthen(FHTNWorldState& WS, const UAIStyleData* Style, TArray<UHTNTaskBase*>& OutPlan);
	bool TryWaitOneTime(FHTNWorldState& WS, const UAIStyleData* Style, TArray<UHTNTaskBase*>& OutPlan);

	bool TryDefendWithCard(FHTNWorldState& WS, const UAIStyleData* Style, int32 AreaIdx, TArray<UHTNTaskBase*>& OutPlan);
	bool TryDefendWithReinforce(FHTNWorldState& WS, const UAIStyleData* Style, int32 AreaIdx, TArray<UHTNTaskBase*>& OutPlan);
	bool TryDefendWithEmploy(FHTNWorldState& WS, const UAIStyleData* Style, int32 AreaIdx, TArray<UHTNTaskBase*>& OutPlan);

    bool TryCaptureDirectAttack(FHTNWorldState& WS, const UAIStyleData* Style, int32 TargetIdx, TArray<UHTNTaskBase*>& OutPlan);
    bool TryCaptureWithReinforce(FHTNWorldState& WS, const UAIStyleData* Style, int32 TargetIdx, TArray<UHTNTaskBase*>& OutPlan);
    bool TryCaptureWithCard(FHTNWorldState& WS,const UAIStyleData* Style,int32 TargetIdx, TArray<UHTNTaskBase*>& OutPlan);

    bool TryBluff(FHTNWorldState& WS, const UAIStyleData* Style, TArray<UHTNTaskBase*>& OutPlan);
    bool TryReinforce(FHTNWorldState& WS,const UAIStyleData* Style, TArray<UHTNTaskBase*>& OutPlan);
    bool TryManageCards(FHTNWorldState& WS, const UAIStyleData* Style, TArray<UHTNTaskBase*>& OutPlan);
    bool TryPreemptiveScan(FHTNWorldState& WS, const UAIStyleData* Style, TArray<UHTNTaskBase*>& OutPlan);
    bool TryStrengthenWithEmploy(FHTNWorldState& WS, const UAIStyleData* Style, TArray<UHTNTaskBase*>& OutPlan);

    int32 FindBestAttackTarget(const FHTNWorldState& WS, const UAIStyleData* Style) const;
    int32 FindMostThreatenedArea(const FHTNWorldState& WS, const UAIStyleData* Style) const;
    bool NeedScan(const FHTNWorldState& WS, const UAIStyleData* Style, int32 AreaIdx) const;
    float CalcThreatLevel(const FHTNWorldState& WS, int32 AreaIdx) const;

    UHTNTaskBase* CreateAttackTask(int32 Src, int32 Dest, float Units);
    UHTNTaskBase* CreateMoveTask(int32 Src, int32 Dest, float Units);
    UHTNTaskBase* CreateScanTask(int32 Target);
    UHTNTaskBase* CreateEmployTask(int32 Target);
    UHTNTaskBase* CreateUseCardTask(int32 CardIdx, int32 Target);
    UHTNTaskBase* CreateDrawCardTask();
    UHTNTaskBase* CreateDiscardCardTask(int32 CardIdx);
    UHTNTaskBase* CreateWaitTask();

    FHTNAreaState BuildAreaState(AAreaBase* Area, FGenericTeamId MyTeamId, float CurrentTime) const;
    void BuildConnection(const TArray<AAreaBase*>& AllAreas, FHTNWorldState& OutState) const;
    void CollectResourceState(AStaAIController* Owner, FHTNWorldState& OutState) const;

    FHTNWorldState CurrentWorldState;
    FHTNWorldState PlannedWorldState;

    UPROPERTY()
    TArray<UHTNTaskBase*> CurrentPlan;
    int32 CurrentPlanIndex = 0;
    bool bPlanValid = false;

    TMap<TWeakObjectPtr<AAreaBase>, float> ScanHistory;

};
