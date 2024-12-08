// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EItemState.h"
#include "Inventory/FGFItem.h"
#include "GFItemManagerComponent.generated.h"

class UGFActorItemDefinition;
class AGFItemActor;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GAMEPLAYFRAMEWORK_API UGFItemManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UGFItemManagerComponent();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(Server,Reliable,BlueprintCallable,Category = "ItemManager")
	virtual void SpawnItemInWorld(const FGFItem& InItem,UGFActorItemDefinition* ActorItemDefinition,const EItemState InItemState);

	UFUNCTION(Client,Reliable,BlueprintCallable,Category = "ItemManager")
	virtual void FailedToSpawnItemInWorld(const FGFItem& InItem,UGFActorItemDefinition* ActorItemDefinition,const EItemState InItemState);

protected:

private:
	UPROPERTY(EditDefaultsOnly,Replicated,Category = "GameplayFramework|Items")
	TArray<AGFItemActor*> ItemsSpawnedInWorld;

	UPROPERTY(EditDefaultsOnly,Category = "GameplayFramework|Items")
	uint8 MaxItemsSpawnedInWorld = 4;
	
};
