// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemManager/GSItemActor.h"

#include "GameplaySystems.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Character.h"
#include "ItemManager/GSGenericItemInfo.h"
#include "Net/UnrealNetwork.h"

AGSItemActor::AGSItemActor()
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	bReplicates = true;
	AActor::SetReplicateMovement(true);

	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>("ItemMesh");
	SetRootComponent(ItemMesh);

	BoxCollision = CreateDefaultSubobject<UBoxComponent>("BoxCollision");
	BoxCollision->SetupAttachment(ItemMesh);

	BoxCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	BoxCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AGSItemActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AGSItemActor,Item)
	DOREPLIFETIME(AGSItemActor,ItemState)
	DOREPLIFETIME(AGSItemActor,ActorItemDefinition)
}

void AGSItemActor::SetItem(const FGSItem& InItem)
{
	Item = InItem;
	
	if (const ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner()))
	{
		ThirdPersonMesh = OwnerCharacter->GetMesh();
	}
}

void AGSItemActor::SetItemState(const EItemState InItemState)
{
	ItemState = InItemState;

	switch (ItemState) {
	case EItemState::None:
		break;
	case EItemState::Equipped:
		Equipped();
		MulticastEquipped();
		break;
	case EItemState::Unequipped:
		Unequipped();
		MulticastUnequipped();
		break;
	case EItemState::Dropped:
		Dropped();
		MulticastDropped();
		break;
	}
}

void AGSItemActor::SetActorItemDefinition(UGSActorItemDefinition* InActorItemDefinition)
{
	ActorItemDefinition = InActorItemDefinition;

	SetItemMesh();
}

void AGSItemActor::Equipped()
{
	if (!BoxCollision->GetCollisionEnabled() == ECR_Ignore)
	{
		BoxCollision->SetCollisionResponseToAllChannels(ECR_Ignore);
		BoxCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	
	AttachItemToCharacterMesh(ItemMesh,ThirdPersonMesh,ActorItemDefinition->SocketForAttachToEquipped);
}

void AGSItemActor::Unequipped()
{
	if (!BoxCollision->GetCollisionEnabled() == ECR_Ignore)
	{
		BoxCollision->SetCollisionResponseToAllChannels(ECR_Ignore);
		BoxCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	
	AttachItemToCharacterMesh(ItemMesh,ThirdPersonMesh,ActorItemDefinition->SocketForAttachToUnequipped);
}

void AGSItemActor::Dropped()
{
	BoxCollision->SetCollisionResponseToChannel(ECC_Interact,ECR_Overlap);
	BoxCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	ItemMesh->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
}

void AGSItemActor::MulticastUnequipped_Implementation()
{
	PlayAnimMontage(ActorItemDefinition->UnequipAnimMontage);
}

void AGSItemActor::MulticastEquipped_Implementation()
{
	PlayAnimMontage(ActorItemDefinition->EquipAnimMontage);
}

void AGSItemActor::MulticastDropped_Implementation()
{
	//...
}

void AGSItemActor::OnRep_Item()
{
	if (const ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner()))
	{
		ThirdPersonMesh = OwnerCharacter->GetMesh();
	}
}

void AGSItemActor::OnRep_ItemState()
{
	switch (GetItemState()) {
	case EItemState::None:
		break;
	case EItemState::Equipped:
		Equipped();
		break;
	case EItemState::Unequipped:
		Unequipped();
		break;
	case EItemState::Dropped:
		Dropped();
		break;
	}
}

void AGSItemActor::OnRep_ItemDefinition()
{
	SetItemMesh();
}

void AGSItemActor::SetItemMesh()
{
	if (ActorItemDefinition->GenericItemInfo->ItemMesh)
	{
        GetItemMeshComponent()->SetStaticMesh(ActorItemDefinition->GenericItemInfo->ItemMesh);
	}
}

void AGSItemActor::PlayAnimMontage(UAnimMontage* InAnimMontage)
{
	if (ThirdPersonMesh && InAnimMontage)
	{
		ThirdPersonMesh->GetAnimInstance()->Montage_Play(InAnimMontage);
	}
}

void AGSItemActor::AttachItemToCharacterMesh(USceneComponent* ComponentToAttach,USceneComponent* CharacterMesh,const FName SocketName)
{
	if (CharacterMesh && ComponentToAttach)
	{
		ComponentToAttach->AttachToComponent(CharacterMesh,FAttachmentTransformRules::SnapToTargetIncludingScale,SocketName);
	}
}

