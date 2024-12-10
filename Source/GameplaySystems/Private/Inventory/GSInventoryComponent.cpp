// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/GSInventoryComponent.h"

#include "Inventory/GSItemCustomCondition.h"
#include "Net/UnrealNetwork.h"


UGSInventoryComponent::UGSInventoryComponent()
{
	
	PrimaryComponentTick.bCanEverTick = false;
	PrimaryComponentTick.bStartWithTickEnabled = false;
	SetIsReplicatedByDefault(true);
}

void UGSInventoryComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(UGSInventoryComponent, Inventory,COND_OwnerOnly)
	DOREPLIFETIME_CONDITION(UGSInventoryComponent, Slots,COND_OwnerOnly)
}

void UGSInventoryComponent::AddInitialItems(const TArray<FGSItem>& InitialItems)
{

	for (const FGSItem& InitialITem : InitialItems)
	{
		
		uint8 Remaining;
		PickUpItem(InitialITem,Remaining);
	}
}

bool UGSInventoryComponent::PickUpItem(const FGSItem& NewItem, uint8& Remaining,const TSubclassOf<UGSItemCustomCondition> CustomCondition)
{
	if (!GetOwner()->HasAuthority())
	{
		ServerPickUpItem(NewItem,CustomCondition);
	}

	if (CustomCondition)
	{
		UGSItemCustomCondition* NewCondition = NewObject<UGSItemCustomCondition>(this,CustomCondition);
		const bool bCanPickupItem = NewCondition->CanPickupItem(this,NewItem);
		if (bCanPickupItem)
		{
			Inventory.Add(NewItem);
		}
		NewCondition->ConditionalBeginDestroy();
		return bCanPickupItem;
	}
	
	uint8 RemainingStack = NewItem.Quantity;
	
	for (FGSItem& ItemData : Inventory)
	{
		if (ItemData.ItemDefinition->GetItemID() == NewItem.ItemDefinition->GetItemID() && ItemData.Quantity < ItemData.ItemDefinition->GetMaxStack())
		{
			const uint8 StackSpace = ItemData.ItemDefinition->GetMaxStack() - ItemData.Quantity;
			const uint8 ValueToAdd = FMath::Min(StackSpace, RemainingStack);

			ItemData.Quantity += ValueToAdd;
			RemainingStack -= ValueToAdd;

			if (RemainingStack == 0)
			{
				Remaining = 0;
				return true;
			}
		}
	}
	
	while (RemainingStack > 0 && Inventory.Num() < Slots)
	{
		const uint8 ValueToAdd = FMath::Min(NewItem.ItemDefinition->GetMaxStack(), RemainingStack);
		const uint8 IndexForNewItem = Inventory.Emplace(NewItem.ItemDefinition, ValueToAdd);
		RemainingStack -= ValueToAdd;
	}
	
	Remaining = RemainingStack;
	return RemainingStack == 0;
}

void UGSInventoryComponent::ConsumeItem(const uint8 ItemID, const uint8 Quantity)
{
	if (Quantity == 0) return;
	
	if (!GetOwner()->HasAuthority())
	{
		ServerConsumeItem(ItemID,Quantity);
	}
	
	uint8 RemainingQuantity = Quantity;

	for (int32 i = Inventory.Num() - 1; i >= 0; --i)
	{
		FGSItem& ConsumeItem = Inventory[i];
		if (ConsumeItem.ItemDefinition->GetItemID() == ItemID)
		{
			if (ConsumeItem.Quantity > RemainingQuantity)
			{
				ConsumeItem.Quantity -= RemainingQuantity;
				return;
			}
			
			RemainingQuantity -= ConsumeItem.Quantity;
			Inventory.RemoveAt(i);
			
			if (RemainingQuantity == 0) return;
		}
	}
}

void UGSInventoryComponent::ServerPickUpItem_Implementation(const FGSItem& NewItem, const TSubclassOf<UGSItemCustomCondition> CustomCondition)
{
	uint8 Remaining;
	PickUpItem(NewItem,Remaining,CustomCondition);
}

void UGSInventoryComponent::ServerConsumeItem_Implementation(const uint8 ItemID, const uint8 Quantity)
{
	ConsumeItem(ItemID,Quantity);
}
