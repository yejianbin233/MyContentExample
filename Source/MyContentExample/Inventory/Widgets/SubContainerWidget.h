// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Blueprint/UserWidget.h"
#include "Inventory/InventoryGameType/Inventory_GameTypes.h"
#include "SubContainerWidget.generated.h"


class UItemSlotWidget;
class UMultiInventoryMainWidget;
/**
 * 子容器控件
 */
UCLASS()
class MYCONTENTEXAMPLE_API USubContainerWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UPROPERTY(BlueprintReadOnly)
	UMultiInventoryMainWidget* InventoryMainWidget;
	
protected:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(ExposeOnSpawn))
	FGameplayTag ContainerNameTag;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(ExposeOnSpawn))
	TEnumAsByte<EContainerType> ContainerType;

	UPROPERTY(BlueprintReadWrite)
	TArray<UItemSlotWidget*> ItemWidgetSlots;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(ExposeOnSpawn))
	TSubclassOf<UItemSlotWidget> ItemSlotWidgetClass;
	// 加载功能

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(ExposeOnSpawn))
	int32 Column;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(ExposeOnSpawn))
	int32 MaxCapacity;
	// 管理界面操作

public:

	UFUNCTION(BlueprintCallable)
	void InitWidget(UMultiInventoryMainWidget* InMultiInventoryMainWidget);

	UFUNCTION(BlueprintImplementableEvent)
	void InitSubContainer();

	UFUNCTION(BlueprintCallable)
	void UpdateSubContain(TArray<UInventoryItemInstance*> SubContainerItemInstances);
};
