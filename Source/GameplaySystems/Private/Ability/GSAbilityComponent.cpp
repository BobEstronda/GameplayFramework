// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/GSAbilityComponent.h"

#include "Ability/GSAbility.h"
#include "Engine/ActorChannel.h"
#include "Net/UnrealNetwork.h"


UGSAbilityComponent::UGSAbilityComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	PrimaryComponentTick.bStartWithTickEnabled = false;
	SetIsReplicatedByDefault(true);
}

void UGSAbilityComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(UGSAbilityComponent,Abilities,COND_OwnerOnly)
}

bool UGSAbilityComponent::ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags)
{
	bool WroteSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);
	for (UGSAbility* Ability : Abilities)
	{
		if (Ability)
		{
			WroteSomething |= Channel->ReplicateSubobject(Ability,*Bunch,*RepFlags);
		}
	}
	return WroteSomething;
}

UGSAbility* UGSAbilityComponent::GetAbilityById(const uint8 AbilityID) const
{
	for (UGSAbility* Ability : Abilities)
	{
		if (Ability->GetAbilityID() == AbilityID)
		{
			return Ability;
		}
	}
	return nullptr;
}

UGSAbility* UGSAbilityComponent::GetAbilityByName(const FName AbilityName) const
{
	for (UGSAbility* Ability : Abilities)
	{
		if (Ability->GetAbilityName() == AbilityName)
		{
			return Ability;
		}
	}
	return nullptr;
}

bool UGSAbilityComponent::AddAbility(const TSubclassOf<UGSAbility> Ability)
{
	if (!GetOwner()->HasAuthority())
	{
		ServerAddAbility(Ability);
	}
	
	for (const UGSAbility* OtherAbility : Abilities)
	{
		if (OtherAbility->GetAbilityID() == Ability.GetDefaultObject()->GetAbilityID())
		{
			return false;
		}
	}

	UGSAbility* NewAbility = NewObject<UGSAbility>(this,Ability);
	Abilities.Add(NewAbility);
	return true;
}

bool UGSAbilityComponent::ActivateAbility(UGSAbility* Ability)
{
	if (!Ability || Ability->IsActivated()) return false;

	if (!GetOwner()->HasAuthority())
	{
		ServerActivateAbility(Ability);
	}
	
	Ability->Activate({GetOwner(),true});
	return true;
}

bool UGSAbilityComponent::DisableAbility(UGSAbility* Ability)
{
	if (!Ability && !Ability->IsActivated()) return false;

	if (!GetOwner()->HasAuthority())
	{
		ServerDisableAbility(Ability);
	}

	Ability->Disable( {GetOwner(),false});
	return true;
}

bool UGSAbilityComponent::ActivateAbilityById(const uint8 AbilityID)
{
	if (!GetOwner()->HasAuthority())
	{
		ServerActivateAbilityById(AbilityID);
	}

	for (UGSAbility* Ability : Abilities)
	{
		if (Ability->GetAbilityID() == AbilityID && !Ability->IsActivated())
		{
			Ability->Activate( {GetOwner(),true});
			return true;
		}
	}
	
	return false;
}

bool UGSAbilityComponent::DisableAbilityById(const uint8 AbilityID)
{
	
	if (!GetOwner()->HasAuthority())
	{
		ServerDisableAbilityById(AbilityID);
	}
	
	for (UGSAbility* Ability : Abilities)
	{
		if (Ability->GetAbilityID() == AbilityID)
		{
			Ability->Disable( {GetOwner(),false});
			return true;
		}
	}
	
	return false;
}

void UGSAbilityComponent::RemoveAbilityById(const uint8 AbilityID)
{
	if (!GetOwner()->HasAuthority())
	{
		ServerRemoveAbilityById(AbilityID);
	}

	for (int32 i = Abilities.Num() - 1; i >= 0; --i)
	{
		UGSAbility* AbilityToRemove = Abilities[i];
		
		if (AbilityToRemove->GetAbilityID() == AbilityID && !AbilityToRemove->IsActivated())
		{
			AbilityToRemove->ConditionalBeginDestroy();
			Abilities.RemoveAt(i);
			return;
		}
	}
}

void UGSAbilityComponent::RemoveAbility(UGSAbility* AbilityToRemove)
{
	if(!AbilityToRemove || AbilityToRemove->IsActivated()) return;

	if (!GetOwner()->HasAuthority())
	{
		ServerRemoveAbility(AbilityToRemove);
	}

	if (Abilities.Contains(AbilityToRemove))
	{
		AbilityToRemove->ConditionalBeginDestroy();
		Abilities.Remove(AbilityToRemove);
	}
}

void UGSAbilityComponent::ServerAddAbility_Implementation(TSubclassOf<UGSAbility> Ability)
{
	AddAbility(Ability);
}

void UGSAbilityComponent::ServerActivateAbility_Implementation(UGSAbility* Ability)
{
	ActivateAbility(Ability);
}

void UGSAbilityComponent::ServerDisableAbility_Implementation(UGSAbility* Ability)
{
	DisableAbility(Ability);
}

void UGSAbilityComponent::ServerActivateAbilityById_Implementation(const uint8 AbilityID)
{
	ActivateAbilityById(AbilityID);
}

void UGSAbilityComponent::ServerDisableAbilityById_Implementation(const uint8 AbilityID)
{
	DisableAbilityById(AbilityID);
}

void UGSAbilityComponent::ServerRemoveAbilityById_Implementation(const uint8 AbilityID)
{
	RemoveAbilityById(AbilityID);
}

void UGSAbilityComponent::ServerRemoveAbility_Implementation(UGSAbility* AbilityToRemove)
{
	RemoveAbility(AbilityToRemove);
}
