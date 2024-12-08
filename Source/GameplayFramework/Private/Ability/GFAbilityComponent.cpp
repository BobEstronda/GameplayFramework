// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/GFAbilityComponent.h"

#include "Ability/GFAbility.h"
#include "Engine/ActorChannel.h"
#include "Net/UnrealNetwork.h"


UGFAbilityComponent::UGFAbilityComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	PrimaryComponentTick.bStartWithTickEnabled = false;
	SetIsReplicatedByDefault(true);
}

void UGFAbilityComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(UGFAbilityComponent,Abilities,COND_OwnerOnly)
}

bool UGFAbilityComponent::ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags)
{
	bool WroteSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);
	for (UGFAbility* Ability : Abilities)
	{
		if (Ability)
		{
			WroteSomething |= Channel->ReplicateSubobject(Ability,*Bunch,*RepFlags);
		}
	}
	return WroteSomething;
}

UGFAbility* UGFAbilityComponent::GetAbilityById(const uint8 AbilityID) const
{
	for (UGFAbility* Ability : Abilities)
	{
		if (Ability->GetAbilityID() == AbilityID)
		{
			return Ability;
		}
	}
	return nullptr;
}

UGFAbility* UGFAbilityComponent::GetAbilityByName(const FName AbilityName) const
{
	for (UGFAbility* Ability : Abilities)
	{
		if (Ability->GetAbilityName() == AbilityName)
		{
			return Ability;
		}
	}
	return nullptr;
}

bool UGFAbilityComponent::AddAbility(const TSubclassOf<UGFAbility> Ability)
{
	if (!GetOwner()->HasAuthority())
	{
		ServerAddAbility(Ability);
	}
	
	for (const UGFAbility* OtherAbility : Abilities)
	{
		if (OtherAbility->GetAbilityID() == Ability.GetDefaultObject()->GetAbilityID())
		{
			return false;
		}
	}

	UGFAbility* NewAbility = NewObject<UGFAbility>(this,Ability);
	Abilities.Add(NewAbility);
	return true;
}

bool UGFAbilityComponent::ActivateAbility(UGFAbility* Ability)
{
	if (!Ability || Ability->IsActivated()) return false;

	if (!GetOwner()->HasAuthority())
	{
		ServerActivateAbility(Ability);
	}
	
	Ability->Activate({GetOwner(),true});
	return true;
}

bool UGFAbilityComponent::DisableAbility(UGFAbility* Ability)
{
	if (!Ability && !Ability->IsActivated()) return false;

	if (!GetOwner()->HasAuthority())
	{
		ServerDisableAbility(Ability);
	}

	Ability->Disable( {GetOwner(),false});
	return true;
}

bool UGFAbilityComponent::ActivateAbilityById(const uint8 AbilityID)
{
	if (!GetOwner()->HasAuthority())
	{
		ServerActivateAbilityById(AbilityID);
	}

	for (UGFAbility* Ability : Abilities)
	{
		if (Ability->GetAbilityID() == AbilityID && !Ability->IsActivated())
		{
			Ability->Activate( {GetOwner(),true});
			return true;
		}
	}
	
	return false;
}

bool UGFAbilityComponent::DisableAbilityById(const uint8 AbilityID)
{
	
	if (!GetOwner()->HasAuthority())
	{
		ServerDisableAbilityById(AbilityID);
	}
	
	for (UGFAbility* Ability : Abilities)
	{
		if (Ability->GetAbilityID() == AbilityID)
		{
			Ability->Disable( {GetOwner(),false});
			return true;
		}
	}
	
	return false;
}

void UGFAbilityComponent::RemoveAbilityById(const uint8 AbilityID)
{
	if (!GetOwner()->HasAuthority())
	{
		ServerRemoveAbilityById(AbilityID);
	}

	for (int32 i = Abilities.Num() - 1; i >= 0; --i)
	{
		UGFAbility* AbilityToRemove = Abilities[i];
		
		if (AbilityToRemove->GetAbilityID() == AbilityID && !AbilityToRemove->IsActivated())
		{
			AbilityToRemove->ConditionalBeginDestroy();
			Abilities.RemoveAt(i);
			return;
		}
	}
}

void UGFAbilityComponent::RemoveAbility(UGFAbility* AbilityToRemove)
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

void UGFAbilityComponent::ServerAddAbility_Implementation(TSubclassOf<UGFAbility> Ability)
{
	AddAbility(Ability);
}

void UGFAbilityComponent::ServerActivateAbility_Implementation(UGFAbility* Ability)
{
	ActivateAbility(Ability);
}

void UGFAbilityComponent::ServerDisableAbility_Implementation(UGFAbility* Ability)
{
	DisableAbility(Ability);
}

void UGFAbilityComponent::ServerActivateAbilityById_Implementation(const uint8 AbilityID)
{
	ActivateAbilityById(AbilityID);
}

void UGFAbilityComponent::ServerDisableAbilityById_Implementation(const uint8 AbilityID)
{
	DisableAbilityById(AbilityID);
}

void UGFAbilityComponent::ServerRemoveAbilityById_Implementation(const uint8 AbilityID)
{
	RemoveAbilityById(AbilityID);
}

void UGFAbilityComponent::ServerRemoveAbility_Implementation(UGFAbility* AbilityToRemove)
{
	RemoveAbility(AbilityToRemove);
}
