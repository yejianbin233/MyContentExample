// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventoryList.generated.h"

class UItemStaticData;
class UInventoryItemInstance;

// 自定义 FInventoryList，用于网络复制。

// 复制数组 - FFastArraySerializerItem
USTRUCT(BlueprintType)
struct FInventoryListItem : public FFastArraySerializerItem
{
	GENERATED_BODY()

public:
	
	UPROPERTY(BlueprintReadOnly)
	UInventoryItemInstance* ItemInstance = nullptr;
	
};

// Note：在按照官方提供的步骤（FastArraySerializer.h）实现后，发现编译错误，是由于没有引入模块依赖导致的，需要在 .build 中引入"NetCore"模块。
USTRUCT(BlueprintType)
struct FInventoryList : public FFastArraySerializer
{
	GENERATED_BODY()

	bool NetDeltaSerialize(FNetDeltaSerializeInfo & DeltaParms)
	{
		return FFastArraySerializer::FastArrayDeltaSerialize<FInventoryListItem, FInventoryList>( Items, DeltaParms, *this );
	}

	void AddItem(TSubclassOf<UItemStaticData> InItemStaticDataClass);

	void AddItem(UInventoryItemInstance* InItemInstance);
	
	void RemoveItem(TSubclassOf<UItemStaticData> InItemStaticDataClass);
	
	void RemoveItem(UInventoryItemInstance* InItemInstance);
	
	TArray<FInventoryListItem>& GetItemsRef(){ return Items; };
	
	UPROPERTY(BlueprintReadOnly)
	TArray<FInventoryListItem> Items;
};

template<>
struct TStructOpsTypeTraits<FInventoryList> : public TStructOpsTypeTraitsBase2<FInventoryList>
{
	enum 
	{
		WithNetDeltaSerializer = true,
   };
};

