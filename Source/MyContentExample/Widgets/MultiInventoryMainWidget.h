// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Blueprint/UserWidget.h"
#include "MultiInventoryMainWidget.generated.h"

class UInventoryComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnItemInteractive, const FHitResult&, Hit);

// 子仓库信息数据
USTRUCT(BlueprintType)
struct FSubContainerInfoData
{
	GENERATED_BODY()

public:
	// 子仓库唯一命名标签，用于检索
	UPROPERTY(BlueprintReadWrite)
	FGameplayTag SubContainerNameTag;

	// 子仓库物品数据，由于在用户控件中，仅作为显示及界面操作，因此不需要复制，只需要仓库组件进行数据复制。
	UPROPERTY(BlueprintReadWrite)
	TArray<FInventoryListItem> Items;
};


/**
 * 多仓库用户控件，可模块化
 */
UCLASS()
class MYCONTENTEXAMPLE_API UMultiInventoryMainWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TArray<FSubContainerInfoData> SubContainerInfoDatas;

	UPROPERTY(BlueprintReadOnly)
	UInventoryComponent* InventoryComponent;

public:

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void MultiContainerDragInteractiveHandle(const FMultiContainerDragInteractiveData& MultiContainerDragInteractiveData);
};
