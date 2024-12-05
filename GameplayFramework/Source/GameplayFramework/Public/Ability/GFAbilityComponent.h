// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Ability/GFAbility.h"
#include "Components/ActorComponent.h"
#include "GFAbilityComponent.generated.h"

class UGFAbility;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GAMEPLAYFRAMEWORK_API UGFAbilityComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UGFAbilityComponent();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	virtual bool ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags) override;

	UFUNCTION(BlueprintCallable,BlueprintPure, Category = "Ability|Getter")
	UGFAbility* GetAbilityById(const uint8 AbilityID) const;

	UFUNCTION(BlueprintCallable,BlueprintPure, Category = "Ability|Getter")
	UGFAbility* GetAbilityByName(const FName AbilityName) const;

	UFUNCTION(BlueprintCallable, Category = "Ability|Client")
	virtual bool AddAbility(const TSubclassOf<UGFAbility> Ability);

	UFUNCTION(BlueprintCallable, Category = "Ability|Client")
	virtual bool ActivateAbility(UGFAbility* Ability);

	UFUNCTION(BlueprintCallable, Category = "Ability|Client")
	virtual bool DisableAbility(UGFAbility* Ability);

	UFUNCTION(BlueprintCallable, Category = "Ability|Client|ID")
	virtual bool ActivateAbilityById(const uint8 AbilityID);

	UFUNCTION(BlueprintCallable, Category = "Ability|Client|ID")
	virtual bool DisableAbilityById(const uint8 AbilityID);

	UFUNCTION(BlueprintCallable, Category = "Ability|Client|ID")
	virtual void RemoveAbilityById(const uint8 AbilityID);

	UFUNCTION(BlueprintCallable, Category = "Ability|Client")
	virtual void RemoveAbility(UGFAbility* AbilityToRemove);
protected:
	UFUNCTION(Server,Reliable)
	void ServerAddAbility(TSubclassOf<UGFAbility> Ability);

	UFUNCTION(Server,Reliable)
	void ServerActivateAbilityById(const uint8 AbilityID);

	UFUNCTION(Server,Reliable)
	void ServerDisableAbilityById(const uint8 AbilityID);

	UFUNCTION(Server,Reliable)
	void ServerRemoveAbilityById(const uint8 AbilityID);

	UFUNCTION(Server,Reliable)
	void ServerActivateAbility(UGFAbility* Ability);

	UFUNCTION(Server,Reliable)
	void ServerDisableAbility(UGFAbility* Ability);

	UFUNCTION(Server,Reliable)
	void ServerRemoveAbility(UGFAbility* AbilityToRemove);

private:
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	TArray<UGFAbility*> Abilities;

		
};
