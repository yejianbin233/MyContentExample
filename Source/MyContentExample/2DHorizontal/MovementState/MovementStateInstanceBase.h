// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MovementStateInstanceBase.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class MYCONTENTEXAMPLE_API UMovementStateInstanceBase : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY()
	UHori2DCharacterMovementComponent* Owner;
	
protected:

	UPROPERTY()
	TEnumAsByte<EHorizontal2DMovementMode> MovementMode;

	UPROPERTY()
	TEnumAsByte<EMovementStateType> MovementStateType;
	
public:

	UMovementStateInstanceBase();
	
	// 处理输入，由于使用了插件管理 2D Flipbook 动画状态机，因此 InputHandle 暂时不处理相应的逻辑。
	UFUNCTION(BlueprintCallable)
	virtual void InputHandle(EMovementStateType MovementState);

	UFUNCTION(BlueprintCallable)
	virtual UMovementStateInstanceBase* SwitchTargetStateInstance(EMovementStateType MovementState);

	UFUNCTION(BlueprintCallable)
	EHorizontal2DMovementMode GetMovementMode() const { return MovementMode; };

	UFUNCTION(BlueprintCallable)
	EMovementStateType GetMovementStateType() const { return MovementStateType; };
	
protected:
	
	virtual void StateChangedHandle(EMovementStateType MovementState);
};
