// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BlueprintFunctionLibrarys/BLF_Inventory.h"
#include "InventoryGameType/Inventory_GameTypes.h"
#include "InventoryItemInstance.generated.h"

class AItemActor;
/**
 * UItemStaticData - 物品静态数据对象
 */
UCLASS(BlueprintType, Blueprintable)
class UItemStaticData : public UObject
{
	GENERATED_BODY()

public:

	// 物品 Id
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Item Static Data")
	int32 Id;
	
	// 物品名称
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Item Static Data")
	FName Name;
	
	// 物品类型
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Item Static Data")
	TEnumAsByte<EItemType> Type;
	
	// 物品在场景中放置时的 Actor 类。
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Item Static Data")
	TSubclassOf<AItemActor> ItemActorClass;
	
	// 冗余设计，用于可装备物品，指定装备后需要附加的插槽
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Item Static Data")
	FName AttachmentSocket = NAME_None;

	// 物品描述
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Item Static Data")
	FString Desc;
	
	// 物品图标
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Item Static Data")
	UTexture2D* Icon;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Item Static Data")
	TEnumAsByte<EItemValue> Value;
	
	// 是否可叠加
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Item Static Data")
	bool IsStackable;
	
	// 叠加量
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Item Static Data")
	int32 StackSize;
};
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
