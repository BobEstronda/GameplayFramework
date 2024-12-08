// Fill out your copyright notice in the Description page of Project Settings.

#include "Equipment/GFEquipmentComponent.h"

#include "Engine/ActorChannel.h"
#include "Equipment/GFEquipment.h"
#include "Net/UnrealNetwork.h"


UGFEquipmentComponent::UGFEquipmentComponent()
{
	
	PrimaryComponentTick.bCanEverTick = false;
	PrimaryComponentTick.bStartWithTickEnabled = false;
	SetIsReplicatedByDefault(true);
}

void UGFEquipmentComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(UGFEquipmentComponent,Equipments,COND_OwnerOnly)
}

bool UGFEquipmentComponent::ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags)
{
	bool WroteSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);
	for (UGFEquipment* Equipment : Equipments)
	{
		if (Equipment)
		{
			WroteSomething |= Channel->ReplicateSubobject(Equipment,*Bunch,*RepFlags);
		}
	}
	return WroteSomething;
}

bool UGFEquipmentComponent::AddEquipment(const TSubclassOf<UGFEquipment> Equipment, const uint8 EquipmentID)
{
	if (!GetOwner()->HasAuthority())
	{
		ServerAddEquipment(Equipment,EquipmentID);
	}
	
	for (const UGFEquipment* OtherEquipment : Equipments)
	{
		if (OtherEquipment->GetEquipmentID() == EquipmentID)
		{
			return false;
		}
	}

	UGFEquipment* NewEquipment = NewObject<UGFEquipment>(this,Equipment);
	NewEquipment->SetEquipmentID(EquipmentID);
	Equipments.Add(NewEquipment);
	return true;
}

bool UGFEquipmentComponent::EquipTheEquipment(const uint8 EquipmentID)
{
	if (!GetOwner()->HasAuthority())
	{
		ServerEquipTheEquipment(EquipmentID);
	}

	for (UGFEquipment* Equipment : Equipments)
	{
		if (Equipment->GetEquipmentID() == EquipmentID && !Equipment->IsEquipped())
		{
			Equipment->EquipmentEquipped(GetOwner());
			return true;
		}
	}
	
	return false;
}

bool UGFEquipmentComponent::UnequipEquipment(const uint8 EquipmentID)
{
	if (!GetOwner()->HasAuthority())
	{
		ServerUnequipEquipment(EquipmentID);
	}
	
	for (UGFEquipment* Equipment : Equipments)
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

void UGFEquipmentComponent::RemoveEquipment(const uint8 EquipmentID)
{
	if (!GetOwner()->HasAuthority())
	{
		ServerRemoveEquipment(EquipmentID);
	}

	for (int32 i = Equipments.Num() - 1; i >= 0; --i)
	{
		UGFEquipment* EquipmentToRemove = Equipments[i];
		
		if (EquipmentToRemove->GetEquipmentID() == EquipmentID && !EquipmentToRemove->IsEquipped())
		{
			EquipmentToRemove->ConditionalBeginDestroy();
			Equipments.RemoveAt(i);
			return;
		}
	}
}

void UGFEquipmentComponent::ServerAddEquipment_Implementation(TSubclassOf<UGFEquipment> Equipment,const uint8 EquipmentID)
{
	AddEquipment(Equipment,EquipmentID);
}

void UGFEquipmentComponent::ServerUnequipEquipment_Implementation(const uint8 EquipmentID)
{
	UnequipEquipment(EquipmentID);
}

void UGFEquipmentComponent::ServerEquipTheEquipment_Implementation(const uint8 EquipmentID)
{
	EquipTheEquipment(EquipmentID);
}

void UGFEquipmentComponent::ServerRemoveEquipment_Implementation(const uint8 EquipmentID)
{
	RemoveEquipment(EquipmentID);
}