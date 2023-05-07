// Fill out your copyright notice in the Description page of Project Settings.


#include "MultiInventoryMainWidget.h"
#include "GameplayTagContainer.h"
#include "GameplayTagsManager.h"
#include "SubContainerWidget.h"
#include "Inventory/InventoryItemInstance.h"
#include "Inventory/Components/InventoryComponent.h"

// 不同子容器具有不同的游戏标签
FGameplayTag SubContainerNameTag_Inventory;
FGameplayTag SubContainerNameTag_Resource;
FGameplayTag SubContainerNameTag_Consumable;
FGameplayTag SubContainerNameTag_CraftingTable;

UMultiInventoryMainWidget::UMultiInventoryMainWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	UGameplayTagsManager::Get().OnLastChanceToAddNativeTags().AddUObject(this, &UMultiInventoryMainWidget::AddGameplayTags);
}

void UMultiInventoryMainWidget::Init(UInventoryComponent* InInventoryComponent)
{
	this->InventoryComponent = InInventoryComponent;

	this->InventoryComponent->InventoryItemDataChanged.AddDynamic(this, &UMultiInventoryMainWidget::UpdateInventoryByComponentData);

	// 初始子容器，不涉及数据。
	K2_InitSubContainer();

	TArray<UInventoryItemInstance*> InventoryItemInstances = InventoryComponent->GetInventoryItemInstances();
	if (InventoryItemInstances.Num() > 0)
	{
		this->UpdateInventoryByComponentData(InventoryItemInstances);
	}

	// 只能在用户控件中才能知道放置了多少个子容器，因此需要在蓝图中进行初始化逻辑编写。
}

void UMultiInventoryMainWidget::UpdateInventoryByComponentData(TArray<UInventoryItemInstance*> InventoryItemInstances)
{
	FilterHandle(InventoryItemInstances);
}

void UMultiInventoryMainWidget::FilterHandle(TArray<UInventoryItemInstance*> InventoryItemInstances)
{
	// 物品具有唯一性，放置在不同的子容器时需要过滤，一旦确认应放置在某个子容器，那么需要将物品排除，不再给其他子容器获取。
	// 且不同的子容器需要有处理顺序，根据子容器的范围大小来排序，范围越小子容器越先处理
	
	if (SubContainerInfoDatas.Contains<FGameplayTag>(SubContainerNameTag_Inventory))
	{
		const FSubContainerInfoData* TargetSubContainer = SubContainerInfoDatas.FindByKey(SubContainerNameTag_Inventory);

		TargetSubContainer->SubContainerWidget->UpdateSubContain(InventoryItemInstances);
	}
	// 其他子容器...
}

void UMultiInventoryMainWidget::AddGameplayTags()
{
	UGameplayTagsManager& TagsManager = UGameplayTagsManager::Get();

	// 向游戏标签管理器添加标签。
	SubContainerNameTag_Inventory = TagsManager.AddNativeGameplayTag("Inventory.SubContainer.Inventory", TEXT("Contain All Item Type."));
	// SubContainerNameTag_Inventory = TagsManager.AddNativeGameplayTag("Event.Inventory.EquipItemActor", TEXT("Equip Item From Item Actor Event."));
	// SubContainerNameTag_Inventory = TagsManager.AddNativeGameplayTag("Event.Inventory.EquipItemActor", TEXT("Equip Item From Item Actor Event."));
}
