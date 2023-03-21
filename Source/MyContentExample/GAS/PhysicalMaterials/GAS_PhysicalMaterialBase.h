// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "GAS_PhysicalMaterialBase.generated.h"

/**
 * 
 */
UCLASS()
class MYCONTENTEXAMPLE_API UGAS_PhysicalMaterialBase : public UPhysicalMaterial
{
	GENERATED_BODY()

public:

	// TODO 为了方便拓展优化，后续可使用虚幻的声音系统模块程序化控制播放左右脚步声音。
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="PhysicalMaterial")
	USoundBase* FootstepSound = nullptr;
};
