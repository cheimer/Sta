#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GenericTeamAgentInterface.h"
#include "FunctionLibrary/AreaCalc.h"
#include "HTNTypes.generated.h"

class UCardData;
class AAreaBase;

UENUM()
enum class EHTNTaskStatus : uint8
{
	Success,
	Failed,
	Running
};

USTRUCT()
struct FHTNAreaState
{
	GENERATED_BODY()

	TWeakObjectPtr<AAreaBase> AreaActor;
	FGenericTeamId OwningTeam;

	float UnitNum = 0.0f;
	float Defense = 1.5f;
	float BluffUnitAdd = 0.0f;
	float BluffDefenseAdd = 0.0f;

	float LastScanTime = 0.0f;

	TArray<int32> ConnectedAreaIndices;

	float GetAreaUnitNum(FGenericTeamId MyTeamId) const
	{
		if (OwningTeam == MyTeamId) return UnitNum;

		return UAreaCalc::CalcBluffUnit(AreaActor.Get());
	}

	float GetAreaDefense(FGenericTeamId MyTeamId) const
	{
		if (OwningTeam == MyTeamId) return Defense;
		
		return UAreaCalc::CalcBluffDefense(AreaActor.Get());
	}
};

USTRUCT()
struct FHTNCardState
{
	GENERATED_BODY()

	TWeakObjectPtr<const UCardData> CardData;
	float Cost;
	FGameplayTag CardTag;
};

USTRUCT()
struct FHTNWorldState
{
    GENERATED_BODY()

    TArray<FHTNAreaState> Areas;

    float CurrentCost = 0.0f;
    float MaxCost = 0.0f;
    TArray<FHTNCardState> HandCards;
    int32 MaxHandSize = 8;
    bool bCanDraw = false;

    FGenericTeamId MyTeamId;

    float CurrentTime = 0.0f;

    TArray<int32> GetMyAreas() const
    {
        TArray<int32> Result;
        for (int32 i = 0; i < Areas.Num(); i++)
        {
            if (Areas[i].OwningTeam == MyTeamId)
            {
            	Result.Add(i);
            }
        }
        return Result;
    }

    TArray<int32> GetEnemyAreas() const
    {
        TArray<int32> Result;
        for (int32 i = 0; i < Areas.Num(); i++)
        {
            if (Areas[i].OwningTeam != MyTeamId && Areas[i].OwningTeam != FGenericTeamId::NoTeam)
            {
                Result.Add(i);
            }
        }
        return Result;
    }

    TArray<int32> GetFrontlineAreas() const
    {
        TArray<int32> Result;
        for (int32 MyAreaIndex : GetMyAreas())
        {
            for (int32 Neighbor : Areas[MyAreaIndex].ConnectedAreaIndices)
            {
                if (Areas[Neighbor].OwningTeam != MyTeamId && Areas[Neighbor].OwningTeam != FGenericTeamId::NoTeam)
                {
                    Result.AddUnique(MyAreaIndex);
                    break;
                }
            }
        }
        return Result;
    }

	TArray<int32> GetRearAreas() const
    {
    	TArray<int32> Frontline = GetFrontlineAreas();
    	TArray<int32> Result;
    	for (int32 MyIdx : GetMyAreas())
    		if (!Frontline.Contains(MyIdx))
    			Result.Add(MyIdx);
    	return Result;
    }
	
    TArray<int32> GetSafeAreas() const
    {
        TArray<int32> Frontline = GetFrontlineAreas();
        TArray<int32> Result;
        for (int32 MyAreaIdx : GetMyAreas())
        {
            if (!Frontline.Contains(MyAreaIdx))
            {
                Result.Add(MyAreaIdx);
            }
        }
        return Result;
    }

    float EstimateAttackResult(int32 SrcIdx, int32 DestIdx) const
    {
        float AttackUnits = Areas[SrcIdx].UnitNum;
        float EnemyDefenseValue = UAreaCalc::CalcGetDefenseValue(Areas[DestIdx].GetAreaUnitNum(MyTeamId), Areas[DestIdx].GetAreaDefense(MyTeamId));
    	
        return AttackUnits - EnemyDefenseValue;
    }
};
