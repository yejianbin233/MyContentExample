// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BlueprintFunctionLibrarys/BLF_Inventory.h"
#include "InventoryGameType/Inventory_GameTypes.h"
#include "InventoryItemInstance.generated.h"

class UItemStaticData;
/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class MYCONTENTEXAMPLE_API UInventoryItemInstance : public UObject
{
	GENERATED_BODY()

public:

	UPROPERTY(Replicated)
	TSubclassOf<UItemStaticData> ItemStaticDataClass;

	UPROPERTY(ReplicatedUsing="OnRep_Equipped")
	bool bEquipped = false;

	UPROPERTY(Replicated)
	AItemActor* ItemActor = nullptr;

public:

	virtual bool IsSupportedForNetworking() const override { return true; };

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty> & OutLifetimeProps ) const override;

	virtual void Init(TSubclassOf<UItemStaticData> InItemStaticDataClass);
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	const UItemStaticData* GetItemStaticData() const { return UBLF_Inventory::GetItemStaticData(ItemStaticDataClass); };

	UFUNCTION()
	void OnRep_Equipped();

	virtual void OnEquipped(AActor* InOwner=nullptr);

	virtual void OnUnEquipped();

	virtual void OnDropped();


protected:

};
