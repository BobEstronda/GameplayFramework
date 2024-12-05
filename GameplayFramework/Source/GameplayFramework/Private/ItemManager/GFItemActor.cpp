// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemManager/GFItemActor.h"

#include "GameplayFramework.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Character.h"
#include "ItemManager/GFGenericItemInfo.h"
#include "Net/UnrealNetwork.h"

AGFItemActor::AGFItemActor()
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

void AGFItemActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AGFItemActor,Item)
	DOREPLIFETIME(AGFItemActor,ItemState)
	DOREPLIFETIME(AGFItemActor,ActorItemDefinition)
}

void AGFItemActor::SetItem(const FGFItem& InItem)
{
	Item = InItem;
	
	if (const ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner()))
	{
		ThirdPersonMesh = OwnerCharacter->GetMesh();
	}
}

void AGFItemActor::SetItemState(const EItemState InItemState)
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

void AGFItemActor::SetActorItemDefinition(UGFActorItemDefinition* InActorItemDefinition)
{
	ActorItemDefinition = InActorItemDefinition;

	SetItemMesh();
}

void AGFItemActor::Equipped()
{
	if (!BoxCollision->GetCollisionEnabled() == ECR_Ignore)
	{
		BoxCollision->SetCollisionResponseToAllChannels(ECR_Ignore);
		BoxCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	
	AttachItemToCharacterMesh(ItemMesh,ThirdPersonMesh,ActorItemDefinition->SocketForAttachToEquipped);
}

void AGFItemActor::Unequipped()
{
	if (!BoxCollision->GetCollisionEnabled() == ECR_Ignore)
	{
		BoxCollision->SetCollisionResponseToAllChannels(ECR_Ignore);
		BoxCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	
	AttachItemToCharacterMesh(ItemMesh,ThirdPersonMesh,ActorItemDefinition->SocketForAttachToUnequipped);
}

void AGFItemActor::Dropped()
{
	BoxCollision->SetCollisionResponseToChannel(ECC_Interact,ECR_Overlap);
	BoxCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	ItemMesh->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
}

void AGFItemActor::MulticastUnequipped_Implementation()
{
	PlayAnimMontage(ActorItemDefinition->UnequipAnimMontage);
}

void AGFItemActor::MulticastEquipped_Implementation()
{
	PlayAnimMontage(ActorItemDefinition->EquipAnimMontage);
}

void AGFItemActor::MulticastDropped_Implementation()
{
	//...
}

void AGFItemActor::OnRep_Item()
{
	if (const ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner()))
	{
		ThirdPersonMesh = OwnerCharacter->GetMesh();
	}
}

void AGFItemActor::OnRep_ItemState()
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

void AGFItemActor::OnRep_ItemDefinition()
{
	SetItemMesh();
}

void AGFItemActor::SetItemMesh()
{
	if (ActorItemDefinition->GenericItemInfo->ItemMesh)
	{
        GetItemMeshComponent()->SetStaticMesh(ActorItemDefinition->GenericItemInfo->ItemMesh);
	}
}

void AGFItemActor::PlayAnimMontage(UAnimMontage* InAnimMontage)
{
	if (ThirdPersonMesh && InAnimMontage)
	{
		ThirdPersonMesh->GetAnimInstance()->Montage_Play(InAnimMontage);
	}
}

void AGFItemActor::AttachItemToCharacterMesh(USceneComponent* ComponentToAttach,USceneComponent* CharacterMesh,const FName SocketName)
{
	if (CharacterMesh && ComponentToAttach)
	{
		ComponentToAttach->AttachToComponent(CharacterMesh,FAttachmentTransformRules::SnapToTargetIncludingScale,SocketName);
	}
}

