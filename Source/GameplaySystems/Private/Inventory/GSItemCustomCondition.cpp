// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/GSItemCustomCondition.h"


bool UGSItemCustomCondition::CanPickupItem_Implementation(const UGSInventoryComponent* InventoryComponent, const FGSItem& ItemForPickup)
{
	return false;
}
