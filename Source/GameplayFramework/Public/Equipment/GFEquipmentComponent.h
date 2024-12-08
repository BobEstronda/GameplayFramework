// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GFEquipmentComponent.generated.h"


class UGFEquipment;

/*
 * Basic Rules
 * 1: You can only add one piece of equipment per Equipment ID.
 * 2: You can only equip one piece of equipment per Equipment ID.
 * 3: You can only remove the equipment if it is unequipped.
 */

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GAMEPLAYFRAMEWORK_API UGFEquipmentComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UGFEquipmentComponent();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	virtual bool ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags) override;

	UFUNCTION(BlueprintCallable, Category = "Equipment|Client")
	virtual bool AddEquipment(const TSubclassOf<UGFEquipment> Equipment, const uint8 EquipmentID);

	UFUNCTION(BlueprintCallable, Category = "Equipment|Client")
	virtual bool EquipTheEquipment(const uint8 EquipmentID);

	UFUNCTION(BlueprintCallable, Category = "Equipment|Client")
	virtual bool UnequipEquipment(const uint8 EquipmentID);

	UFUNCTION(BlueprintCallable, Category = "Equipment|Client")
	virtual void RemoveEquipment(const uint8 EquipmentID);
protected:
	UFUNCTION(Server,Reliable)
	void ServerAddEquipment(TSubclassOf<UGFEquipment> Equipment, const uint8 EquipmentID);

	UFUNCTION(Server,Reliable)
	virtual void ServerEquipTheEquipment(const uint8 EquipmentID);

	UFUNCTION(Server,Reliable)
	virtual void ServerUnequipEquipment(const uint8 EquipmentID);

	UFUNCTION(Server,Reliable)
	virtual void ServerRemoveEquipment(const uint8 EquipmentID);
private:
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	TArray<UGFEquipment*> Equipments;
};
