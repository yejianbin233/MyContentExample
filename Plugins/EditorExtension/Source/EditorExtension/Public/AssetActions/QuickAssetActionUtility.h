// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AssetActionUtility.h"

/* 重命名资产 */
#include "Materials/Material.h"
#include "Materials/MaterialInstance.h"
#include "Particles/ParticleSystem.h"
#include "Sound/SoundCue.h"
#include "Sound/SoundWave.h"
#include "ENGINE/Texture.h"
#include "Blueprint/UserWidget.h"
#include "Components/SkeletalMeshComponent.h"
#include "NiagaraSystem.h"
#include "NiagaraEmitter.h"
#include "Blueprint/UserWidget.h"
/* 重命名资产 */

#include "QuickAssetActionUtility.generated.h"

/**
 * 
 */
UCLASS()
class EDITOREXTENSION_API UQuickAssetActionUtility : public UAssetActionUtility
{
	GENERATED_BODY()

private:

	// 根据资产的类型为资产添加前缀。
	TMap<UClass*, FString> PrefixMap = {
		{UBlueprint::StaticClass(), TEXT("BP_")},
		{UStaticMesh::StaticClass(), TEXT("SM_")},
		{UMaterial::StaticClass(), TEXT("M_")},
		{UMaterialInstance::StaticClass(), TEXT("MI_")},
		{UMaterialFunctionInterface::StaticClass(), TEXT("MF_")},
		{UParticleSystem::StaticClass(), TEXT("PS_")},
		{USoundCue::StaticClass(), TEXT("SC_")},
		{USoundWave::StaticClass(), TEXT("SW_")},
		{UTexture::StaticClass(), TEXT("T_")},
		{UTexture2D::StaticClass(), TEXT("T_")},
		// {UUserWidget::StaticClass(), TEXT("WBP_")}, // 这一行有异常，不知道什么回事。
		{USkeletalMeshComponent::StaticClass(), TEXT("SK_")},
		{UNiagaraSystem::StaticClass(), TEXT("NS_")},
		{UNiagaraEmitter::StaticClass(), TEXT("NE_")},
	};

public:

	// 复制选中的多个资产
	UFUNCTION(CallInEditor)
	void DuplicateAssets(int32 NumOfDuplicates);

	UFUNCTION(CallInEditor)
	void AddPrefixes();
};
