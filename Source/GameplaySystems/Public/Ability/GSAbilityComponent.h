// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Ability/GSAbility.h"
#include "Components/ActorComponent.h"
#include "GSAbilityComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GAMEPLAYSYSTEMS_API UGSAbilityComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UGSAbilityComponent();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	virtual bool ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags) override;

	UFUNCTION(BlueprintCallable,BlueprintPure, Category = "Ability|Getter")
	UGSAbility* GetAbilityById(const uint8 AbilityID) const;

	UFUNCTION(BlueprintCallable,BlueprintPure, Category = "Ability|Getter")
	UGSAbility* GetAbilityByName(const FName AbilityName) const;

	UFUNCTION(BlueprintCallable, Category = "Ability|Client")
	virtual bool AddAbility(const TSubclassOf<UGSAbility> Ability);

	UFUNCTION(BlueprintCallable, Category = "Ability|Client")
	virtual bool ActivateAbility(UGSAbility* Ability);

	UFUNCTION(BlueprintCallable, Category = "Ability|Client")
	virtual bool DisableAbility(UGSAbility* Ability);

	UFUNCTION(BlueprintCallable, Category = "Ability|Client|ID")
	virtual bool ActivateAbilityById(const uint8 AbilityID);

	UFUNCTION(BlueprintCallable, Category = "Ability|Client|ID")
	virtual bool DisableAbilityById(const uint8 AbilityID);

	UFUNCTION(BlueprintCallable, Category = "Ability|Client|ID")
	virtual void RemoveAbilityById(const uint8 AbilityID);

	UFUNCTION(BlueprintCallable, Category = "Ability|Client")
	virtual void RemoveAbility(UGSAbility* AbilityToRemove);
protected:
	UFUNCTION(Server,Reliable)
	void ServerAddAbility(TSubclassOf<UGSAbility> Ability);

	UFUNCTION(Server,Reliable)
	void ServerActivateAbilityById(const uint8 AbilityID);

	UFUNCTION(Server,Reliable)
	void ServerDisableAbilityById(const uint8 AbilityID);

	UFUNCTION(Server,Reliable)
	void ServerRemoveAbilityById(const uint8 AbilityID);

	UFUNCTION(Server,Reliable)
	void ServerActivateAbility(UGSAbility* Ability);

	UFUNCTION(Server,Reliable)
	void ServerDisableAbility(UGSAbility* Ability);

	UFUNCTION(Server,Reliable)
	void ServerRemoveAbility(UGSAbility* AbilityToRemove);

private:
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	TArray<UGSAbility*> Abilities;

		
};
