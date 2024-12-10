// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GSAbility.generated.h"

class AActor;

USTRUCT(BlueprintType)
struct FAbilityRepData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	AActor* Owner = nullptr;

	UPROPERTY(BlueprintReadOnly)
	bool bActivated = false;
};

/**
 * 
 */
UCLASS(Blueprintable,BlueprintType)
class GAMEPLAYSYSTEMS_API UGSAbility : public UObject
{
	GENERATED_BODY()

public:
	virtual bool IsSupportedForNetworking() const override
	{
		return true;
	}

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(BlueprintCallable,BlueprintPure, Category = "Ability|Getter")
	FORCEINLINE bool IsActivated() const {return RepData.bActivated; }

	UFUNCTION(BlueprintCallable,BlueprintPure, Category = "Ability|Getter")
	FORCEINLINE uint8 GetAbilityID() const {return AbilityID; }
	
	UFUNCTION(BlueprintCallable,BlueprintPure, Category = "Ability|Getter")
	FORCEINLINE FName GetAbilityName() const {return AbilityName; }

	UFUNCTION(BlueprintNativeEvent)
	void Activate(const FAbilityRepData& AbilityData);

	UFUNCTION(BlueprintNativeEvent)
	void Disable(const FAbilityRepData& AbilityData);

protected:
	UFUNCTION() void OnRep_RepData();

private:
	UPROPERTY(ReplicatedUsing = OnRep_RepData)
	FAbilityRepData RepData;

	UPROPERTY(EditAnywhere,Category = "Ability")
	FName AbilityName;

	UPROPERTY(EditAnywhere,Category = "Ability")
	uint8 AbilityID;
};
