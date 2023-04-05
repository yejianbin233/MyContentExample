// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "2DHorizontal/MovementState/MovementStateInstanceBase.h"
#include "MovementStateInstanceJump.generated.h"

/**
 * 
 */
UCLASS()
class MYCONTENTEXAMPLE_API UMovementStateInstanceJump : public UMovementStateInstanceBase
{
	GENERATED_BODY()

public:
	UMovementStateInstanceJump();
	virtual void InputHandle(EMovementStateType MovementState) override;
	virtual UMovementStateInstanceBase* SwitchTargetStateInstance(EMovementStateType MovementState) override;

protected:
	virtual void StateChangedHandle(EMovementStateType MovementState) override;
};
