// Fill out your copyright notice in the Description page of Project Settings.


#include "Equipment/GSEquipment.h"

#include "Equipment/GSFragment.h"
#include "Net/UnrealNetwork.h"

void UGSEquipment::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	UObject::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME_CONDITION(UGSEquipment,EquipmentID,COND_OwnerOnly)
	DOREPLIFETIME_CONDITION_NOTIFY(UGSEquipment,RepData,COND_None,REPNOTIFY_OnChanged)
}

void UGSEquipment::EquipmentEquipped_Implementation(AActor* Owner)
{
	RepData.bEquipped = true;
	RepData.Owner = Owner;
	
	for (const TSubclassOf<UGSFragment> Fragment : Fragments)
	{
		if (!Fragment)
		{
			continue;
		}
		UGSFragment* NewFragment = NewObject<UGSFragment>(Owner,Fragment);
		NewFragment->FragmentActivated(RepData.Owner);
		ActiveFragments.Add(NewFragment);
	}

	Fragments.Empty();
}

bool UGSEquipment::EquipmentUnequipped_Implementation(AActor* Owner)
{
	RepData.bEquipped = false;
	RepData.Owner = Owner;
	
	for (UGSFragment* DisableFragment : ActiveFragments)
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

void UGSEquipment::OnRep_RepData()
{
	if (IsEquipped())
	{
		EquipmentEquipped(RepData.Owner);
		return;
	}
	EquipmentUnequipped(RepData.Owner);
}
