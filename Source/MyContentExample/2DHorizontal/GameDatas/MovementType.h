// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MovementType.generated.h"

UENUM(BlueprintType)
enum class EMovementStateType : uint8
{
	OnMove UMETA(DisplayName = "移动"),
	/* 地面运动 */
	OnGroundIdle UMETA(DisplayName = "空闲"),
	
	OnGroundWalking UMETA(DisplayName = "地面行走"),

	OnLanded UMETA(DisplayName = "着陆"),
	/* 地面运动 */

	/* 空中运动 */
	OnJumping UMETA(DisplayName = "跳跃"),

	OnFalling UMETA(DisplayName = "下落"),

	OnToGrabing UMETA(DisplayName = "抓住墙壁"),
	
	/* 空中运动 */

	/*墙壁运动*/
	OnGrabingGlissade UMETA(DisplayName = "墙壁滑行"),
	
	/*墙壁运动*/
	
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
enum class EHorizontal2DMovementMode : uint8
{
	Hor2D_MOVE_GroundWalking UMETA(DisplayName = "地面行走"),
	Hor2D_Jumping UMETA(DisplayName = "跳跃"),
	Hor2D_Climbing_Vine UMETA(DisplayName = "藤蔓/梯子攀爬"),
	Hor2D_Climbing_Wall UMETA(DisplayName = "墙壁攀爬"),
	Hor2D_Falling UMETA(DisplayName = "下落"),
	Hor2D_Landed UMETA(DisplayName = "着陆"),
};

