// // Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Inventory_GameTypes.generated.h"

// 物体类型
UENUM(BlueprintType)
enum class EItemType : uint8
{
	Resource UMETA(DisplayName = "资源"),
	Equipable UMETA(DisplayName = "可装备"),
	Armor UMETA(DisplayName = "装甲"),
	Consumable UMETA(DisplayName = "可消耗"),
	Buildable UMETA(DisplayName = "可建造")
};

// 物体价值
UENUM(BlueprintType)
enum class EItemValue : uint8
{
	Common UMETA(DisplayName = "普通"),
	Uncommon UMETA(DisplayName = "罕见"),
	Rare UMETA(DisplayName = "稀有"),
	Epic UMETA(DisplayName = "史诗"),
	Legendary UMETA(DisplayName = "传奇")
};

// 装甲类型
UENUM(BlueprintType)
enum class EItemArmorType : uint8
{
	Helmet UMETA(DisplayName = "头盔"),
	Chest UMETA(DisplayName = "胸甲"),
	Pants UMETA(DisplayName = "裤子"),
	Boots UMETA(DisplayName = "靴子")
};

// 物体实例信息（运行时可改变）
USTRUCT(BlueprintType)
struct FItemInstanceInfo
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, Category="Item Info")
	int32 Quantity;
};
