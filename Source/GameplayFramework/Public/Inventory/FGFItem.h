#pragma once

#include "CoreMinimal.h"
#include "Inventory/GFItemDefinition.h"
#include "FGFItem.generated.h"

USTRUCT(BlueprintType,Blueprintable)
struct FGFItem
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	const UGFItemDefinition* ItemDefinition = nullptr;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	uint8 Quantity = 0;

	bool operator==(const FGFItem& Other) const
	{
		return ItemDefinition->GetItemID() == Other.ItemDefinition->GetItemID();
	}
};
