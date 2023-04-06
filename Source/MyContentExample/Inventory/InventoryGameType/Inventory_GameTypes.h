// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Inventory_GameTypes.generated.h"

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
