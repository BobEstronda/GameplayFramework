// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Inventory/FGSItem.h"
#include "GSInventoryComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnNewItemAdded,const FGSItem&, NewItem, bool, Success);

/*
 * Basic Rules
 * 1: Each item must have its own unique ItemIndex. Reason: Comparison in the struct is based on its ItemIndex.
 * 2: The max stack also represents your maximum amount in a slot (UI visual part).
 * If your stack reaches its maximum, then a new item must be added to the inventory, in turn it will occupy its own slot.
 * 3: If your slots reach the maximum value you can no longer collect items.
 */

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GAMEPLAYSYSTEMS_API UGSInventoryComponent : public UActorComponent
{
	GENERATED_BODY()
	
	UPROPERTY(VisibleAnywhere,Replicated,Category = "InventorySystem|Items")
	TArray<FGSItem> Inventory;

	UPROPERTY(EditDefaultsOnly,Replicated,Category = "InventorySystem|Slots")
	uint8 MaxSlotsInInventory = 10;

public:	
	UGSInventoryComponent();
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(BlueprintCallable,BlueprintPure)
	const TArray<FGSItem>& GetInventory() const {return Inventory; }
	
	UFUNCTION(BlueprintCallable,BlueprintPure)
	const uint8 GetMaxSlotsInInventory() const {return MaxSlotsInInventory; }

	UFUNCTION(Server,Reliable, BlueprintCallable, Category = "Inventory|Server")
	void AddInitialItems(const TArray<FGSItem>& InitialItems);

	UFUNCTION(BlueprintCallable, Category = "Inventory|Client")
	virtual void PickUpItem(const FGSItem& NewItem, uint8& Remaining);

	UFUNCTION(BlueprintCallable, Category = "Inventory|Client")
	virtual void ConsumeItem(const uint8 ItemID, const uint8 Quantity);

	UPROPERTY(BlueprintAssignable, BlueprintReadOnly)
	FOnNewItemAdded NewItemAdded;

protected:
	UFUNCTION(Server,Reliable, Category = "Inventory|Server")
	virtual void ServerPickUpItem(const FGSItem& NewItem);

	UFUNCTION(Server,Reliable, Category = "Inventory|Server")
	virtual void ServerConsumeItem(const uint8 ItemID, const uint8 Quantity);

	UFUNCTION(Client,Reliable)
	virtual void ClientNewItemAdded(const FGSItem& NewItem,bool Success);
};
