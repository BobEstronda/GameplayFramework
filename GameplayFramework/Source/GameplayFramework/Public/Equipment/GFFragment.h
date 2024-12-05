// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GFFragment.generated.h"


/**
 * 
 */
UCLASS(Blueprintable,BlueprintType)
class GAMEPLAYFRAMEWORK_API UGFFragment : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent)
	void FragmentActivated(AActor* Owner);

	UFUNCTION(BlueprintNativeEvent)
	bool FragmentDisabled(AActor* Owner);
	
};
