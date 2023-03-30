// Fill out your copyright notice in the Description page of Project Settings.


#include "BlueprintFunctionLibrarys/BLF_Inventory.h"

#include "Inventory/InventoryGameType/Inventory_GameTypes.h"

const UItemStaticData* UBLF_Inventory::GetItemStaticData(TSubclassOf<UItemStaticData> ItemStaticDataClass)
{
	if (IsValid(ItemStaticDataClass))
	{
		return GetDefault<const UItemStaticData>(ItemStaticDataClass);
	}
	
	return nullptr;
}
