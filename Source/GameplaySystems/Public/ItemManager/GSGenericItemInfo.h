// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GSGenericItemInfo.generated.h"

/**
 * 
 */
UCLASS(Blueprintable,BlueprintType)
class GAMEPLAYSYSTEMS_API UGSGenericItemInfo : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	UStaticMesh* ItemMesh = nullptr;
};
