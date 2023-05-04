// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Blueprint/DragDropOperation.h"
#include "MultiContainerDragDropOperation.generated.h"

/**
 * 可拖动操作类
 */
UCLASS(Blueprintable, BlueprintType)
class MYCONTENTEXAMPLE_API UMultiContainerDragDropOperation : public UDragDropOperation
{
	GENERATED_BODY()

public:

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, meta=(ExposeOnSpawn))
	TEnumAsByte<EContainerType> FromContainer;

	UPROPERTY(BlueprintReadWrite, meta=(ExposeOnSpawn))
	FGameplayTag FromContainerNameTag;

	UPROPERTY(BlueprintReadWrite, meta=(ExposeOnSpawn))
	int32 FromContainerSlotIndex;
};
