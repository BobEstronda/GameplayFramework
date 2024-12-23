// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/GSInventoryComponent.h"

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
	DOREPLIFETIME_CONDITION(UGSInventoryComponent, MaxSlotsInInventory,COND_OwnerOnly)
}

void UGSInventoryComponent::AddInitialItems_Implementation(const TArray<FGSItem>& InitialItems)
{
	
}

void UGSInventoryComponent::PickUpItem(const FGSItem& NewItem, uint8& Remaining)
{
	if (!GetOwner()->HasAuthority())
	{
		ServerPickUpItem(NewItem);
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
				Remaining = RemainingStack;
				return;
			}
		}
	}
	
	while (RemainingStack > 0 && Inventory.Num() < MaxSlotsInInventory)
	{
		const uint8 ValueToAdd = FMath::Min(NewItem.ItemDefinition->GetMaxStack(), RemainingStack);
		const int32 IndexForNewItem = Inventory.Emplace(NewItem.ItemDefinition, ValueToAdd);
		RemainingStack -= ValueToAdd;
		
		if (GetOwner()->HasAuthority())
		{
			ClientNewItemAdded(Inventory[IndexForNewItem],RemainingStack == 0);
		}
	}
	
	Remaining = RemainingStack;
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

void UGSInventoryComponent::ClientNewItemAdded_Implementation(const FGSItem& NewItem, bool Success)
{
	NewItemAdded.Broadcast(NewItem,Success);
}

void UGSInventoryComponent::ServerPickUpItem_Implementation(const FGSItem& NewItem)
{
	uint8 Remaining;
	PickUpItem(NewItem,Remaining);
}

void UGSInventoryComponent::ServerConsumeItem_Implementation(const uint8 ItemID, const uint8 Quantity)
{
	ConsumeItem(ItemID,Quantity);
}
