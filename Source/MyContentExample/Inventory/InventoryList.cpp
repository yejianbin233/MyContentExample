// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryList.h"
#include "InventoryItemInstance.h"
#include "InventoryGameType/Inventory_GameTypes.h"

void FInventoryList::AddItem(TSubclassOf<UItemStaticData> InItemStaticDataClass)
{
	FInventoryListItem& Item = Items.AddDefaulted_GetRef();
	Item.ItemInstance = NewObject<UInventoryItemInstance>();

	Item.ItemInstance->Init(InItemStaticDataClass);
	// 在添加物品后必须调用 MarkItemDirty() 函数
	MarkItemDirty(Item);

	
}

void FInventoryList::AddItem(UInventoryItemInstance* InItemInstance)
{
	FInventoryListItem& Item = Items.AddDefaulted_GetRef();
	Item.ItemInstance = InItemInstance;
	MarkItemDirty(Item);
	
}

void FInventoryList::RemoveItem(TSubclassOf<UItemStaticData> InItemStaticDataClass)
{
	for (auto ItemIter = Items.CreateIterator(); ItemIter; ++ItemIter)
	{
		FInventoryListItem& Item = *ItemIter;

		if (Item.ItemInstance && Item.ItemInstance->GetItemStaticData()->IsA(InItemStaticDataClass))
		{
			ItemIter.RemoveCurrent();

			// 在修改数组后，必须调用 "MarkArrayDirty"，标记数组为脏
			MarkArrayDirty();

			break;
		}
	}
}

void FInventoryList::RemoveItem(UInventoryItemInstance* InItemInstance)
{
	for (auto ItemIter = Items.CreateIterator(); ItemIter; ++ItemIter)
	{
		FInventoryListItem& Item = *ItemIter;

		if (Item.ItemInstance && Item.ItemInstance == InItemInstance)
		{
			// TODO - 如何更新仓库物品数据?
			// ItemIter.RemoveCurrent();

			// 在修改数组后，必须调用 "MarkArrayDirty"，标记数组为脏
			MarkArrayDirty();

			break;
		}
	}
}
