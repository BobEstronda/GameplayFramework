// Fill out your copyright notice in the Description page of Project Settings.


#include "Equipment/GFEquipment.h"

#include "Equipment/GFFragment.h"
#include "Net/UnrealNetwork.h"

void UGFEquipment::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	UObject::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME_CONDITION(UGFEquipment,EquipmentID,COND_OwnerOnly)
	DOREPLIFETIME_CONDITION_NOTIFY(UGFEquipment,RepData,COND_None,REPNOTIFY_OnChanged)
}

void UGFEquipment::EquipmentEquipped_Implementation(AActor* Owner)
{
	RepData.bEquipped = true;
	RepData.Owner = Owner;
	
	for (const TSubclassOf<UGFFragment> Fragment : Fragments)
	{
		if (!Fragment)
		{
			continue;
		}
		UGFFragment* NewFragment = NewObject<UGFFragment>(Owner,Fragment);
		NewFragment->FragmentActivated(RepData.Owner);
		ActiveFragments.Add(NewFragment);
	}

	Fragments.Empty();
}

bool UGFEquipment::EquipmentUnequipped_Implementation(AActor* Owner)
{
	RepData.bEquipped = false;
	RepData.Owner = Owner;
	
	for (UGFFragment* DisableFragment : ActiveFragments)
	{
		if (DisableFragment->FragmentDisabled(RepData.Owner))
		{
			DisableFragment->ConditionalBeginDestroy();
		}
		return false;
	}
	
	ActiveFragments.Empty();
	return true;
}

void UGFEquipment::OnRep_RepData()
{
	if (IsEquipped())
	{
		EquipmentEquipped(RepData.Owner);
		return;
	}
	EquipmentUnequipped(RepData.Owner);
}
