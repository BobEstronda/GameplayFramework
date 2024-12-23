// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemManager/GSItemManagerComponent.h"
#include "ItemManager/GSItemActor.h"
#include "Net/UnrealNetwork.h"

UGSItemManagerComponent::UGSItemManagerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	PrimaryComponentTick.bStartWithTickEnabled = false;
	SetIsReplicatedByDefault(true);
}

void UGSItemManagerComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UGSItemManagerComponent, ItemsSpawnedInWorld)
	DOREPLIFETIME(UGSItemManagerComponent, EquippedItem)
}

void UGSItemManagerComponent::SetEquippedItem(AGSItemActor* InItem)
{
	if (EquippedItem)
	{
		EquippedItem->SetItemState(EItemState::Unequipped);
	}
	EquippedItem = InItem;
	EquippedItem->SetItemState(EItemState::Equipped);
}

void UGSItemManagerComponent::FailedToSpawnItemInWorld_Implementation(UGSItemDefinition* InItem,UGSActorItemDefinition* ActorItemDefinition, const EItemState InItemState)
{
	//....
}

void UGSItemManagerComponent::SpawnItemInWorld_Implementation(UGSItemDefinition* InItem,UGSActorItemDefinition* ActorItemDefinition,const EItemState InItemState)
{
	if (!ActorItemDefinition) return;
	
	if (ItemsSpawnedInWorld.Num() >= MaxItemsSpawnedInWorld)
	{
		FailedToSpawnItemInWorld(InItem,ActorItemDefinition,InItemState);
		return;
	}
	
	AGSItemActor* ItemInWorld = GetWorld()->SpawnActor<AGSItemActor>(ActorItemDefinition->ActorToWorld,GetOwner()->GetActorLocation(),GetOwner()->GetActorRotation());
	ItemInWorld->SetOwner(GetOwner());
	ItemInWorld->SetItem(InItem);
	ItemInWorld->SetActorItemDefinition(ActorItemDefinition);

	ItemsSpawnedInWorld.Add(ItemInWorld);

	if (InItemState == EItemState::Equipped)
	{
		SetEquippedItem(ItemInWorld);
		return;
	}
	ItemInWorld->SetItemState(InItemState);
}

