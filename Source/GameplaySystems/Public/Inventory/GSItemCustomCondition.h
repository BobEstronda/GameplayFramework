// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Inventory/FGSItem.h"
#include "GSItemCustomCondition.generated.h"

class UGSInventoryComponent;

/**
 * 
 */
UCLASS(Blueprintable,BlueprintType)
class GAMEPLAYSYSTEMS_API UGSItemCustomCondition : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent)
	bool CanPickupItem(const UGSInventoryComponent* InventoryComponent, const FGSItem& ItemForPickup);
};
