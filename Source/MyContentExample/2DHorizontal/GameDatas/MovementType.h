// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MovementType.generated.h"

UENUM(BlueprintType)
enum class EMovementStateType : uint8
{
	OnPlanarWalking UMETA(DisplayName = "平面行走"),
	OnJumping UMETA(DisplayName = "跳跃")
};

UENUM(BlueprintType)
enum class EPlayerInputKeyState : uint8
{
	Hold UMETA(DisplayName = "按住"),
	Pressed UMETA(DisplayName = "按下"),
	Relax UMETA(DisplayName = "松开")
};

/** Movement modes for 2D Horizontal Character. */
UENUM(BlueprintType)
enum EHorizontal2DMovementMode
{
	Hor2D_MOVE_Walking UMETA(DisplayName = "行走"),
	Hor2D_MOVE_Jumping UMETA(DisplayName = "跳跃"),
	Hor2D_Climbing_Vine UMETA(DisplayName = "藤蔓/梯子攀爬"),
	Hor2D_Climbing_Wall UMETA(DisplayName = "墙壁攀爬"),
};

UCLASS(BlueprintType, Blueprintable)
class MYCONTENTEXAMPLE_API UMovementStateInstanceBase : public UObject
{
	GENERATED_BODY()

public:

	// 处理输入，由于使用了插件管理 2D Flipbook 动画状态机，因此 InputHandle 暂时不处理相应的逻辑。
	virtual UMovementStateInstanceBase* InputHandle(EMovementStateType MovementState);

	virtual bool SwitchTargetState(EMovementStateType MovementState);

protected:
	
	virtual void StateChangedHandle(EMovementStateType MovementState);
};

