// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/GFInventoryComponent.h"

#include "Inventory/GFItemCustomCondition.h"
#include "Net/UnrealNetwork.h"


UGFInventoryComponent::UGFInventoryComponent()
{
	
	PrimaryComponentTick.bCanEverTick = false;
	PrimaryComponentTick.bStartWithTickEnabled = false;
	SetIsReplicatedByDefault(true);
}

void UGFInventoryComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(UGFInventoryComponent, Inventory,COND_OwnerOnly)
	DOREPLIFETIME_CONDITION(UGFInventoryComponent, Slots,COND_OwnerOnly)
}

void UGFInventoryComponent::AddInitialItems(const TArray<FGFItem>& InitialItems)
{

	for (const FGFItem& InitialITem : InitialItems)
	{
		
		uint8 Remaining;
		PickUpItem(InitialITem,Remaining);
	}
}

bool UGFInventoryComponent::PickUpItem(const FGFItem& NewItem, uint8& Remaining,const TSubclassOf<UGFItemCustomCondition> CustomCondition)
{
	if (!GetOwner()->HasAuthority())
	{
		ServerPickUpItem(NewItem,CustomCondition);
	}

	if (CustomCondition)
	{
		UGFItemCustomCondition* NewCondition = NewObject<UGFItemCustomCondition>(this,CustomCondition);
		const bool bCanPickupItem = NewCondition->CanPickupItem(this,NewItem);
		if (bCanPickupItem)
		{
			Inventory.Add(NewItem);
		}
		NewCondition->ConditionalBeginDestroy();
		return bCanPickupItem;
	}
	
	uint8 RemainingStack = NewItem.Quantity;
	
	for (FGFItem& ItemData : Inventory)
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

void UGFInventoryComponent::ConsumeItem(const uint8 ItemID, const uint8 Quantity)
{
	if (Quantity == 0) return;
	
	if (!GetOwner()->HasAuthority())
	{
		ServerConsumeItem(ItemID,Quantity);
	}
	
	uint8 RemainingQuantity = Quantity;

	for (int32 i = Inventory.Num() - 1; i >= 0; --i)
	{
		FGFItem& ConsumeItem = Inventory[i];
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

void UGFInventoryComponent::ServerPickUpItem_Implementation(const FGFItem& NewItem, const TSubclassOf<UGFItemCustomCondition> CustomCondition)
{
	uint8 Remaining;
	PickUpItem(NewItem,Remaining,CustomCondition);
}

void UGFInventoryComponent::ServerConsumeItem_Implementation(const uint8 ItemID, const uint8 Quantity)
{
	ConsumeItem(ItemID,Quantity);
}
