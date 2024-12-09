// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Inventory/FGSItem.h"
#include "GSInventoryComponent.generated.h"

class UGSItemCustomCondition;

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

public:	
	UGSInventoryComponent();
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void AddInitialItems(const TArray<FGSItem>& InitialItems);

	UFUNCTION(BlueprintCallable, Category = "Inventory|Client")
	virtual bool PickUpItem(const FGSItem& NewItem, uint8& Remaining,const TSubclassOf<UGSItemCustomCondition> CustomCondition = nullptr);

	UFUNCTION(BlueprintCallable, Category = "Inventory|Client")
	virtual void ConsumeItem(const uint8 ItemID, const uint8 Quantity);

protected:

	UFUNCTION(Server,Reliable)
	virtual void ServerPickUpItem(const FGSItem& NewItem,TSubclassOf<UGSItemCustomCondition> CustomCondition = nullptr);

	UFUNCTION(Server,Reliable)
	virtual void ServerConsumeItem(const uint8 ItemID, const uint8 Quantity);
	
private:
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Replicated,Category = "GameplayFramework|Items",meta=(AllowPrivateAccess = "true"))
	TArray<FGSItem> Inventory;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Replicated,Category = "GameplayFramework|Slots",meta=(AllowPrivateAccess = "true"))
	uint8 Slots = 10;
};
