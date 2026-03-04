// Sta Project - Multiplay RTS with GAS

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "AreaCalc.generated.h"

class AAreaBase;
/**
 * 
 */
UCLASS()
class STA_API UAreaCalc : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	static float CalcBluffUnit(const AAreaBase* Area);
	static float CalcBluffDefense(const AAreaBase* Area);

	static float CalcGetDefenseValue(const float UnitNum, const float Defense);
	static float CalcDefenseValueToUnitNum(const float DefenseValue, const float Defense);
	
};
