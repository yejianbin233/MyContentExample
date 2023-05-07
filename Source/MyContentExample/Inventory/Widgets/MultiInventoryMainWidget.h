// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Blueprint/UserWidget.h"
#include "MultiInventoryMainWidget.generated.h"

class USubContainerWidget;
class UInventoryComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnItemInteractive, const FHitResult&, Hit);

// 子仓库信息数据
USTRUCT(BlueprintType)
struct FSubContainerInfoData
{
	GENERATED_BODY()

public:
	// 子仓库唯一命名标签，用于检索
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FGameplayTag SubContainerNameTag;

	// 子仓库物品数据，由于在用户控件中，仅作为显示及界面操作，因此不需要复制，只需要仓库组件进行数据复制。
	UPROPERTY(BlueprintReadWrite)
	USubContainerWidget* SubContainerWidget;

	bool operator==(const FGameplayTag& OtherTag) const
	{
		if (SubContainerNameTag == OtherTag)
		{
			return true;
		}
		
		return false;
	};
};


/**
 * 多仓库用户控件，可模块化
 */
UCLASS()
class MYCONTENTEXAMPLE_API UMultiInventoryMainWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite)
	TArray<FSubContainerInfoData> SubContainerInfoDatas;

	UPROPERTY(BlueprintReadOnly)
	UInventoryComponent* InventoryComponent;

protected:

	void AddGameplayTags();

public:

	UMultiInventoryMainWidget(const FObjectInitializer& ObjectInitializer);

	void Init(UInventoryComponent* InInventoryComponent);

	UFUNCTION(BlueprintImplementableEvent)
	void K2_InitSubContainer();
	
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void MultiContainerDragInteractiveHandle(const FMultiContainerDragInteractiveData& MultiContainerDragInteractiveData);

	UFUNCTION(BlueprintCallable)
	void UpdateInventoryByComponentData(TArray<UInventoryItemInstance*> InventoryItemInstances);
	
	UFUNCTION(BlueprintCallable)
	void FilterHandle(TArray<UInventoryItemInstance*> InventoryItemInstances);
};
