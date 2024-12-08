// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemManager/GFItemManagerComponent.h"
#include "ItemManager/GFItemActor.h"
#include "Net/UnrealNetwork.h"

UGFItemManagerComponent::UGFItemManagerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	PrimaryComponentTick.bStartWithTickEnabled = false;
	SetIsReplicatedByDefault(true);
	
}

void UGFItemManagerComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UGFItemManagerComponent, ItemsSpawnedInWorld)
}

void UGFItemManagerComponent::FailedToSpawnItemInWorld_Implementation(const FGFItem& InItem,UGFActorItemDefinition* ActorItemDefinition, const EItemState InItemState)
{
	//....
}

void UGFItemManagerComponent::SpawnItemInWorld_Implementation(const FGFItem& InItem,UGFActorItemDefinition* ActorItemDefinition,const EItemState InItemState)
{
	if (!ActorItemDefinition) return;
	
	if (ItemsSpawnedInWorld.Num() >= MaxItemsSpawnedInWorld)
	{
		FailedToSpawnItemInWorld(InItem,ActorItemDefinition,InItemState);
		return;
	}
	
	AGFItemActor* ItemInWorld = GetWorld()->SpawnActor<AGFItemActor>(ActorItemDefinition->ActorToWorld,GetOwner()->GetActorLocation(),GetOwner()->GetActorRotation());
	ItemInWorld->SetOwner(GetOwner());
	ItemInWorld->SetItem(InItem);
	ItemInWorld->SetActorItemDefinition(ActorItemDefinition);
	ItemInWorld->SetItemState(InItemState);
}

