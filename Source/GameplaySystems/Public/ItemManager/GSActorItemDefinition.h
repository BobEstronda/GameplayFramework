// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GSActorItemDefinition.generated.h"

class AGSItemActor;
class UStaticMesh;
class UAnimMontage;
class UGSGenericItemInfo;

/**
 * 
 */
UCLASS(BlueprintType,Blueprintable)
class GAMEPLAYSYSTEMS_API UGSActorItemDefinition : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	TSubclassOf<AGSItemActor> ActorToWorld;
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	UGSGenericItemInfo* GenericItemInfo;

	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "Equipped")
	FName SocketForAttachToEquipped;

	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "Equipped")
	UAnimMontage* EquipAnimMontage;

	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "Unequipped")
	FName SocketForAttachToUnequipped;

	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "Unequipped")
	UAnimMontage* UnequipAnimMontage;
};
