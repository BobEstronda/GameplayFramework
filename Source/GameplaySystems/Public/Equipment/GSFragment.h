// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GSFragment.generated.h"


/**
 * 
 */
UCLASS(Blueprintable,BlueprintType)
class GAMEPLAYSYSTEMS_API UGSFragment : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent)
	void FragmentActivated(AActor* Owner);

	UFUNCTION(BlueprintNativeEvent)
	bool FragmentDisabled(AActor* Owner);
	
};
