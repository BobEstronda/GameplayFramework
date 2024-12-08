#pragma once

UENUM(BlueprintType)
enum class EItemState : uint8
{
	None UMETA(DisplayName = "None"),
	Equipped UMETA(DisplayName = "Equipped"),
	Unequipped UMETA(DisplayName = "Unequipped"),
	Dropped UMETA(DisplayName = "Dropped")
};