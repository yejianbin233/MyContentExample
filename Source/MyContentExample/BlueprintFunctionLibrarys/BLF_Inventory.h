// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "BLF_Inventory.generated.h"

class UItemStaticData;
/**
 * 
 */
UCLASS()
class MYCONTENTEXAMPLE_API UBLF_Inventory : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, BlueprintPure)
	static const UItemStaticData* GetItemStaticData(TSubclassOf<UItemStaticData> ItemStaticDataClass);
	
};
