// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GFGenericItemInfo.generated.h"

/**
 * 
 */
UCLASS(Blueprintable,BlueprintType)
class GAMEPLAYFRAMEWORK_API UGFGenericItemInfo : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	UStaticMesh* ItemMesh = nullptr;
};
