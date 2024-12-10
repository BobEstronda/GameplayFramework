#pragma once

#include "CoreMinimal.h"
#include "Inventory/GSItemDefinition.h"
#include "FGSItem.generated.h"

USTRUCT(BlueprintType,Blueprintable)
struct FGSItem
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	const UGSItemDefinition* ItemDefinition = nullptr;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	uint8 Quantity = 0;

	bool operator==(const FGSItem& Other) const
	{
		return ItemDefinition->GetItemID() == Other.ItemDefinition->GetItemID();
	}
};
