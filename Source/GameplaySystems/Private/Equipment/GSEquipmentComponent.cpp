// Fill out your copyright notice in the Description page of Project Settings.

#include "Equipment/GSEquipmentComponent.h"

#include "Engine/ActorChannel.h"
#include "Equipment/GSEquipment.h"
#include "Net/UnrealNetwork.h"


UGSEquipmentComponent::UGSEquipmentComponent()
{
	
	PrimaryComponentTick.bCanEverTick = false;
	PrimaryComponentTick.bStartWithTickEnabled = false;
	SetIsReplicatedByDefault(true);
}

void UGSEquipmentComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(UGSEquipmentComponent,Equipments,COND_OwnerOnly)
}

bool UGSEquipmentComponent::ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags)
{
	bool WroteSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);
	for (UGSEquipment* Equipment : Equipments)
	{
		if (Equipment)
		{
			WroteSomething |= Channel->ReplicateSubobject(Equipment,*Bunch,*RepFlags);
		}
	}
	return WroteSomething;
}

bool UGSEquipmentComponent::AddEquipment(const TSubclassOf<UGSEquipment> Equipment, const uint8 EquipmentID)
{
	if (!GetOwner()->HasAuthority())
	{
		ServerAddEquipment(Equipment,EquipmentID);
	}
	
	for (const UGSEquipment* OtherEquipment : Equipments)
	{
		if (OtherEquipment->GetEquipmentID() == EquipmentID)
		{
			return false;
		}
	}

	UGSEquipment* NewEquipment = NewObject<UGSEquipment>(this,Equipment);
	NewEquipment->SetEquipmentID(EquipmentID);
	Equipments.Add(NewEquipment);
	return true;
}

bool UGSEquipmentComponent::EquipTheEquipment(const uint8 EquipmentID)
{
	if (!GetOwner()->HasAuthority())
	{
		ServerEquipTheEquipment(EquipmentID);
	}

	for (UGSEquipment* Equipment : Equipments)
	{
		if (Equipment->GetEquipmentID() == EquipmentID && !Equipment->IsEquipped())
		{
			Equipment->EquipmentEquipped(GetOwner());
			return true;
		}
	}
	
	return false;
}

bool UGSEquipmentComponent::UnequipEquipment(const uint8 EquipmentID)
{
	if (!GetOwner()->HasAuthority())
	{
		ServerUnequipEquipment(EquipmentID);
	}
	
	for (UGSEquipment* Equipment : Equipments)
	{
		if (Equipment->GetEquipmentID() == EquipmentID)
		{
			if (Equipment->EquipmentUnequipped(GetOwner()))
			{
				return true;
			}
			return false;
		}
	}
	
	return false;
}

void UGSEquipmentComponent::RemoveEquipment(const uint8 EquipmentID)
{
	if (!GetOwner()->HasAuthority())
	{
		ServerRemoveEquipment(EquipmentID);
	}

	for (int32 i = Equipments.Num() - 1; i >= 0; --i)
	{
		UGSEquipment* EquipmentToRemove = Equipments[i];
		
		if (EquipmentToRemove->GetEquipmentID() == EquipmentID && !EquipmentToRemove->IsEquipped())
		{
			EquipmentToRemove->ConditionalBeginDestroy();
			Equipments.RemoveAt(i);
			return;
		}
	}
}

void UGSEquipmentComponent::ServerAddEquipment_Implementation(TSubclassOf<UGSEquipment> Equipment,const uint8 EquipmentID)
{
	AddEquipment(Equipment,EquipmentID);
}

void UGSEquipmentComponent::ServerUnequipEquipment_Implementation(const uint8 EquipmentID)
{
	UnequipEquipment(EquipmentID);
}

void UGSEquipmentComponent::ServerEquipTheEquipment_Implementation(const uint8 EquipmentID)
{
	EquipTheEquipment(EquipmentID);
}

void UGSEquipmentComponent::ServerRemoveEquipment_Implementation(const uint8 EquipmentID)
{
	RemoveEquipment(EquipmentID);
}