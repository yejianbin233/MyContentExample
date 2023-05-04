// // Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Inventory_GameTypes.generated.h"

// 物品类型
UENUM(BlueprintType)
enum class EItemType : uint8
{
	Resource UMETA(DisplayName = "资源"),
	Equip UMETA(DisplayName = "装备"),
	// Armor UMETA(DisplayName = "装甲"),
	Consumable UMETA(DisplayName = "消耗品"),
	// ConsumableWithDurability UMETA(DisplayName = "可消耗耐久"),
	// Buildable UMETA(DisplayName = "可建造"),
};

// 物品价值
UENUM(BlueprintType)
enum class EItemValue : uint8
{
	Common UMETA(DisplayName = "普通"),
	Uncommon UMETA(DisplayName = "罕见"),
	Rare UMETA(DisplayName = "稀有"),
	Epic UMETA(DisplayName = "史诗"),
	Legendary UMETA(DisplayName = "传奇")
};

// 装备类型
UENUM(BlueprintType)
enum class EItemArmorType : uint8
{
	Helmet UMETA(DisplayName = "头盔"),
	Chest UMETA(DisplayName = "胸甲"),
	Pants UMETA(DisplayName = "裤子"),
	Boots UMETA(DisplayName = "靴子"),
	Weapon UMETA(DisplayName = "武器"),
};

// 物品状态
UENUM(BlueprintType)
enum class EItemState : uint8
{
	None UMETA(DisplayName = "无状态"),
	Equipped UMETA(DisplayName = "已装备"),
};

// 物品实例信息（运行时可改变）
USTRUCT(BlueprintType)
struct FItemInstanceInfo
{ 
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, Category="Item Info")
	int32 Quantity;

	UPROPERTY(BlueprintReadWrite, Category="Item Info")
	TEnumAsByte<EItemState> ItemState;
};

// 物品容器类型
UENUM(BlueprintType)
enum class EContainerType : uint8
{
	ShortcutBar UMETA(DisplayName = "快捷栏"),
	Knapsack UMETA(DisplayName = "背包"),
	EquipmentBar UMETA(DisplayName = "装备栏")
};

