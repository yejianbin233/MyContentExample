// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AssetActionUtility.h"
#include "QuickAssetActionUtility.generated.h"

/**
 * 
 */
UCLASS()
class EDITOREXTENSION_API UQuickAssetActionUtility : public UAssetActionUtility
{
	GENERATED_BODY()

public:

	// 复制选中的多个资产
	UFUNCTION(CallInEditor)
	void DuplicateAssets(int32 NumOfDuplicates);
};
