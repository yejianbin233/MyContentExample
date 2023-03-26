// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS_GameTypes.generated.h"

class UGameplayEffect;
class UGameplayAbility;

USTRUCT(BlueprintType)
struct FGASCharacterData
{
	GENERATED_BODY();

	// 默认授予的技能（数组）
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GAS", DisplayName="游戏效果数组")
	TArray<TSubclassOf<UGameplayEffect>> Effects;

	// 默认应用的游戏效果（数组）
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GAS", DisplayName="游戏技能数组")
	TArray<TSubclassOf<UGameplayAbility>> Abilities;
};

USTRUCT(BlueprintType)
struct FGASCharacterAnimData
{
	GENERATED_BODY();

	// 站立移动混合空间
	UPROPERTY(EditDefaultsOnly)
	class UBlendSpace* MovementBlendspace = nullptr;

	// 站立状态空闲动画
	UPROPERTY(EditDefaultsOnly)
	class UAnimSequenceBase* IdleAnimationAsset = nullptr;

	// 蹲伏状态移动混合空间
	UPROPERTY(EditDefaultsOnly)
	UBlendSpace* CrouchMovementBlendspace = nullptr;

	// 蹲伏状态空间动画
	UPROPERTY(EditDefaultsOnly)
	UAnimSequenceBase* CrouchIdleAnimationAsset = nullptr;
};

UENUM(BlueprintType)
enum class EFoot : uint8
{
	Left UMETA(DisplayName = "Left"),
	Right UMETA(DisplayName = "Right")
};

