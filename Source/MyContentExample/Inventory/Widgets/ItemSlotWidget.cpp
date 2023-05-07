// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemSlotWidget.h"

#include "MultiInventoryMainWidget.h"
#include "SubContainerWidget.h"

void UItemSlotWidget::InitWidget(USubContainerWidget* InSubContainerWidget)
{
	this->SubContainerWidget = InSubContainerWidget;

	if (SubContainerWidget->InventoryMainWidget)
	{
		// 绑定拖动交互事件
		OnItemDragInteractive.AddDynamic(SubContainerWidget->InventoryMainWidget, &UMultiInventoryMainWidget::MultiContainerDragInteractiveHandle);
	};

	SetWidgetDefaultVisibility();
}

void UItemSlotWidget::DragInteractiveHandle(const FMultiContainerDragInteractiveData& MultiContainerDragInteractiveData)
{
	OnItemDragInteractive.Broadcast(MultiContainerDragInteractiveData);
}
