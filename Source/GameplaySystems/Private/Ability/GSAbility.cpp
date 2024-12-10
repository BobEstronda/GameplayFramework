// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/GSAbility.h"

#include "Net/UnrealNetwork.h"

void UGSAbility::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	UObject::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UGSAbility,RepData,COND_None,REPNOTIFY_OnChanged)
}

void UGSAbility::Activate_Implementation(const FAbilityRepData& AbilityData)
{
	RepData = AbilityData;
}

void UGSAbility::Disable_Implementation(const FAbilityRepData& AbilityData)
{
	RepData = AbilityData;
}

void UGSAbility::OnRep_RepData()
{
	if (IsActivated())
	{
		Activate(RepData);
		return;
	}
	Disable(RepData);
}
