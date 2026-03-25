// Sta Project - Multiplay RTS with GAS


#include "AreaCalc.h"

#include "AbilitySystem/AttributeSet/AreaAttributeSet.h"
#include "Area/AreaBase.h"

float UAreaCalc::CalcBluffUnit(const AAreaBase* Area)
{
	if (!Area || !Area->GetAttributeSet()) return -1.0f;
	
	float UnitNum = Area->GetAttributeSet()->GetUnitNum();
	float BluffAdd = Area->GetAttributeSet()->GetBluffUnitAdd();

	return UnitNum + BluffAdd;
}

float UAreaCalc::CalcBluffDefense(const AAreaBase* Area)
{
	if (!Area || !Area->GetAttributeSet()) return -1.0f;
	
	float Defense = Area->GetAttributeSet()->GetDefense();
	float BluffAdd = Area->GetAttributeSet()->GetBluffDefenseAdd();

	return Defense + BluffAdd;
}

float UAreaCalc::CalcGetDefenseValue(const float UnitNum, const float Defense)
{
	return UnitNum * Defense;
}

float UAreaCalc::CalcDefenseValueToUnitNum(const float DefenseValue, const float Defense)
{
	return FMath::FloorToInt(DefenseValue * (1 / Defense));
}
