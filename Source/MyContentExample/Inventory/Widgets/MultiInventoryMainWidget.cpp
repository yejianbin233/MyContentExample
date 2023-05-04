// Fill out your copyright notice in the Description page of Project Settings.


#include "MultiInventoryMainWidget.h"

#include "Inventory/Components/InventoryComponent.h"

void UMultiInventoryMainWidget::Init(UInventoryComponent* InInventoryComponent)
{
	this->InventoryComponent = InventoryComponent;

	this->InventoryComponent->InventoryItemDataChanged.AddDynamic(this, &UMultiInventoryMainWidget::UpdateInventoryByComponentData);
}
