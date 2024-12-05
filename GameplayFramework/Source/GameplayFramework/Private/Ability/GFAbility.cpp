// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/GFAbility.h"

#include "Net/UnrealNetwork.h"

void UGFAbility::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	UObject::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UGFAbility,RepData,COND_None,REPNOTIFY_OnChanged)
}

void UGFAbility::Activate_Implementation(const FAbilityRepData& AbilityData)
{
	RepData = AbilityData;
}

void UGFAbility::Disable_Implementation(const FAbilityRepData& AbilityData)
{
	RepData = AbilityData;
}

void UGFAbility::OnRep_RepData()
{
	if (IsActivated())
	{
		Activate(RepData);
		return;
	}
	Disable(RepData);
}
