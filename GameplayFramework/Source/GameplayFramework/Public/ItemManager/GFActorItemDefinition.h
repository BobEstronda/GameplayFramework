// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GFActorItemDefinition.generated.h"

class AGFItemActor;
class UStaticMesh;
class UAnimMontage;
class UGFGenericItemInfo;

/**
 * 
 */
UCLASS(BlueprintType,Blueprintable)
class GAMEPLAYFRAMEWORK_API UGFActorItemDefinition : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	TSubclassOf<AGFItemActor> ActorToWorld;
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	UGFGenericItemInfo* GenericItemInfo;

	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "Equipped")
	FName SocketForAttachToEquipped;

	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "Equipped")
	UAnimMontage* EquipAnimMontage;

	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "Unequipped")
	FName SocketForAttachToUnequipped;

	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "Unequipped")
	UAnimMontage* UnequipAnimMontage;
};
