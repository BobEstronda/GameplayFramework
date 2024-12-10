// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GSEquipment.generated.h"

class UGSFragment;
class AActor;

USTRUCT()
struct FEquipmentRepData
{
	GENERATED_BODY()

	UPROPERTY()
	AActor* Owner = nullptr;

	UPROPERTY()
	bool bEquipped = false;
};

/**
 * 
 */
UCLASS(Blueprintable,BlueprintType)
class GAMEPLAYSYSTEMS_API UGSEquipment : public UObject
{
	GENERATED_BODY()

public:
	
	virtual bool IsSupportedForNetworking() const override
	{
		return true;
	}

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(BlueprintCallable,BlueprintPure)
	FORCEINLINE bool IsEquipped() const {return RepData.bEquipped; }

	UFUNCTION(BlueprintCallable,BlueprintPure)
	FORCEINLINE uint8 GetEquipmentID() const {return EquipmentID; }

	FORCEINLINE void SetEquipmentID(const uint8 NewEquipmentID) {EquipmentID = NewEquipmentID; }
	
	UFUNCTION(BlueprintNativeEvent)
	void EquipmentEquipped(AActor* Owner);

	UFUNCTION(BlueprintNativeEvent)
	bool EquipmentUnequipped(AActor* Owner);

protected:
	UFUNCTION() void OnRep_RepData();
	
private:
	UPROPERTY(EditAnywhere, Category = "Equipment|Fragments")
	TArray<TSubclassOf<UGSFragment>> Fragments;

	UPROPERTY()
	TArray<UGSFragment*> ActiveFragments;

	UPROPERTY(ReplicatedUsing = OnRep_RepData)
	FEquipmentRepData RepData;

	UPROPERTY(Replicated)
	uint8 EquipmentID;
};
