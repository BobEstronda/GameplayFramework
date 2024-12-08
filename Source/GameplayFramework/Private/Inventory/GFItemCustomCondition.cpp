// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/GFItemCustomCondition.h"


bool UGFItemCustomCondition::CanPickupItem_Implementation(const UGFInventoryComponent* InventoryComponent, const FGFItem& ItemForPickup)
{
	return false;
}
