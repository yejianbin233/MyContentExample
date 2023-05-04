// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Blueprint/UserWidget.h"
#include "ItemSlotWidget.generated.h"

class USubContainerWidget;
class UMultiContainerDragDropOperation;

USTRUCT(BlueprintType)
struct FMultiContainerDragInteractiveData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, meta=(ExposeOnSpawn))
	UMultiContainerDragDropOperation* DragDropOperation;

	UPROPERTY(BlueprintReadWrite, meta=(ExposeOnSpawn))
	TEnumAsByte<EContainerType> DropContainer;

	UPROPERTY(BlueprintReadWrite, meta=(ExposeOnSpawn))
	FGameplayTag DropContainerNameTag;

	UPROPERTY(BlueprintReadWrite, meta=(ExposeOnSpawn))
	int32 DropContainerSlotIndex;

	UPROPERTY(BlueprintReadWrite, meta=(ExposeOnSpawn))
	bool DropResult;
};


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnItemDragInteractive, const FMultiContainerDragInteractiveData&, MultiContainerDragInteractiveData);
/**
 * 
 */
UCLASS()
class MYCONTENTEXAMPLE_API UItemSlotWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadOnly)
	USubContainerWidget* SubContainerWidget;

protected:

	UPROPERTY(BlueprintReadWrite, meta=(ExposeOnSpawn))
	TSubclassOf<UUserWidget> DragWidgetClass;

	// 是否处于拖动状态
	UPROPERTY(BlueprintReadWrite)
	bool IsDragging;
	
	UPROPERTY(BlueprintReadWrite, meta=(ExposeOnSpawn))
	FGameplayTag ContainerNameTag;
	
	UPROPERTY(BlueprintReadWrite, meta=(ExposeOnSpawn))
	TEnumAsByte<EContainerType> ContainerType;

	UPROPERTY(BlueprintReadWrite, meta=(ExposeOnSpawn))
	int32 ContainerSlotIndex;

	UPROPERTY(BlueprintReadWrite)
	FName ItemName;

	UPROPERTY(BlueprintReadWrite)
	UTexture2D* ItemIcon;

	UPROPERTY(BlueprintReadWrite)
	TEnumAsByte<EItemType> ItemType;

	UPROPERTY(BlueprintReadWrite)
	bool IsStackable;

	UPROPERTY(BlueprintReadWrite)
	int32 ItemNum;

	UPROPERTY(BlueprintReadWrite)
	int32 MaxStackNum;

	UPROPERTY(BlueprintReadWrite)
	float DurabilityValue;

	UPROPERTY(BlueprintReadWrite)
	float MaxDurabilityValue;

	UPROPERTY(BlueprintAssignable, BlueprintReadOnly)
	FOnItemDragInteractive OnItemDragInteractive;
	
public:

	UFUNCTION(BlueprintCallable)
	void InitWidget(USubContainerWidget* InSubContainerWidget);

protected:

	// 设置控件的默认可视化内容
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void SetWidgetDefaultVisibility();

	// 控件开始拖动处理
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void WidgetDragStartHandle(UDragDropOperation* Operation);

	// 控件开始拖动 Tick 处理
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void WidgetDragTickHandle(UDragDropOperation* Operation);

	// 控件拖动取消处理
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void WidgetDragCancleHandle(UDragDropOperation* Operation);

	// 控件拖动完成处理
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void WidgetDragCompleteHandle(UDragDropOperation* Operation);

	UFUNCTION(BlueprintCallable)
	void DragInteractiveHandle(const FMultiContainerDragInteractiveData& MultiContainerDragInteractiveData);
};
