// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GSItemUIInfo.generated.h"

class UTexture2D;
class UGSGenericItemInfo;
/**
 * 
 */
UCLASS()
class GAMEPLAYSYSTEMS_API UGSItemUIInfo : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	UTexture2D* Icon = nullptr;

	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	FText Description;

	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	UGSGenericItemInfo* GenericItemInfo;
};
