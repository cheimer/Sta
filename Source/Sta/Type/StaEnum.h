#pragma once

/**
 * Input Types
 */
UENUM(BlueprintType)
enum class EInputPriority : uint8
{
	Character = 0,
	Controller = 1,

	Unknown UMETA(Hidden)
};
