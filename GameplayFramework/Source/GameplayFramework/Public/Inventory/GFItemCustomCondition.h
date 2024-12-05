// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Inventory/FGFItem.h"
#include "GFItemCustomCondition.generated.h"

class UGFInventoryComponent;

/**
 * 
 */
UCLASS(Blueprintable,BlueprintType)
class GAMEPLAYFRAMEWORK_API UGFItemCustomCondition : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent)
	bool CanPickupItem(const UGFInventoryComponent* InventoryComponent, const FGFItem& ItemForPickup);
};
