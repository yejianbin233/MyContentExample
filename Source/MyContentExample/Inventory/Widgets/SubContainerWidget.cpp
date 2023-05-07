// Fill out your copyright notice in the Description page of Project Settings.


#include "SubContainerWidget.h"

#include "ItemSlotWidget.h"

void USubContainerWidget::InitWidget(UMultiInventoryMainWidget* InMultiInventoryMainWidget)
{
	this->InventoryMainWidget = InMultiInventoryMainWidget;

	InitSubContainer();
}

void USubContainerWidget::UpdateSubContain(TArray<UInventoryItemInstance*> SubContainerItemInstances)
{
	for (int Index = 0; Index < SubContainerItemInstances.Num(); ++Index)
	{
		ItemWidgetSlots[Index]->UpdateItemSlot(SubContainerItemInstances[Index]);
	}
}

