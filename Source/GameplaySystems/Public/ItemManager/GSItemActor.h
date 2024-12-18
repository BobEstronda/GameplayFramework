// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemManager/EItemState.h"
#include "ItemManager/GSActorItemDefinition.h"
#include "GSItemActor.generated.h"

class UGSItemDefinition;
class UStaticMeshComponent;
class UBoxComponent;
class UGSActorItemDefinition;

UCLASS()
class GAMEPLAYSYSTEMS_API AGSItemActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AGSItemActor();

	UFUNCTION(BlueprintCallable,BlueprintPure)
	FORCEINLINE UGSItemDefinition* GetItem() const {return Item; }

	UFUNCTION(BlueprintCallable,BlueprintPure)
	FORCEINLINE EItemState GetItemState() const {return ItemState; }

	UFUNCTION(BlueprintCallable,BlueprintPure)
	FORCEINLINE UGSActorItemDefinition* GetActorItemDefinition() const {return ActorItemDefinition; }

	UFUNCTION(BlueprintCallable,BlueprintPure)
	FORCEINLINE UStaticMeshComponent* GetItemMeshComponent() const {return ItemMesh;}

	UFUNCTION(BlueprintCallable,BlueprintPure)
	FORCEINLINE USkeletalMeshComponent* GetThirdPersonMesh() const {return ThirdPersonMesh;}

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	virtual void SetItem(UGSItemDefinition* InItem);
	
	virtual void SetItemState(const EItemState InItemState);

	virtual void SetActorItemDefinition(UGSActorItemDefinition* InActorItemDefinition);

	UFUNCTION(BlueprintCallable, Category = "ItemState")
	virtual void Equipped();
	
	UFUNCTION(BlueprintCallable, Category = "ItemState")
	virtual void Unequipped();

	UFUNCTION(BlueprintCallable, Category = "ItemState")
	virtual void Dropped();

	UFUNCTION(NetMulticast,Reliable,BlueprintCallable, Category = "ItemState")
	virtual void MulticastEquipped();

	UFUNCTION(NetMulticast,Reliable,BlueprintCallable, Category = "ItemState")
	virtual void MulticastUnequipped();

	UFUNCTION(NetMulticast,Reliable,BlueprintCallable, Category = "ItemState")
	virtual void MulticastDropped();
	
protected:

	UFUNCTION()
	virtual void OnRep_Item();

	UFUNCTION()
	virtual void OnRep_ItemState();

	UFUNCTION()
	virtual void OnRep_ItemDefinition();
	
	virtual void SetItemMesh();
	
	virtual void PlayAnimMontage(UAnimMontage* InAnimMontage);
	virtual void AttachItemToCharacterMesh(USceneComponent* ComponentToAttach,USceneComponent* CharacterMesh,const FName SocketName = "None");
	
private:
	UPROPERTY(VisibleAnywhere,Category = "GameplayFramework|Components")
	UStaticMeshComponent* ItemMesh;

	UPROPERTY(VisibleAnywhere,Category = "GameplayFramework|Components")
	UBoxComponent* BoxCollision;

	UPROPERTY(VisibleAnywhere,ReplicatedUsing = OnRep_Item, Category = "GameplayFramework|Components")
	UGSItemDefinition* Item;

	UPROPERTY(VisibleAnywhere,ReplicatedUsing = OnRep_ItemDefinition, Category = "GameplayFramework|ActorItemDefinition")
	UGSActorItemDefinition* ActorItemDefinition = nullptr;

	UPROPERTY(VisibleAnywhere,ReplicatedUsing = OnRep_ItemState, Category = "GameplayFramework|ItemState")
	EItemState ItemState = EItemState::None;
	
	UPROPERTY()
	USkeletalMeshComponent* ThirdPersonMesh;
};
