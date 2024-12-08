// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GFItemDefinition.generated.h"

/**
 * 
 */
UCLASS()
class GAMEPLAYFRAMEWORK_API UGFItemDefinition : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UFUNCTION(Blueprintable,BlueprintPure)
	FORCEINLINE FName GetItemName() const {return ItemName; }

	UFUNCTION(Blueprintable,BlueprintPure)
	FORCEINLINE uint8 GetItemID() const {return ItemID; }

	UFUNCTION(BlueprintCallable,BlueprintPure)
	FORCEINLINE uint8 GetMaxStack() const {return MaxStack; }

private:
	UPROPERTY(EditAnywhere)
	FName ItemName;
	
	UPROPERTY(EditAnywhere)
	uint8 ItemID = 0;
	
	UPROPERTY(EditAnywhere)
	uint8 MaxStack = 1;
};
