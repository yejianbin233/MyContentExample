// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "2DHorizontal/MovementState/MovementStateInstanceBase.h"
#include "MovementStateInstanceGround.generated.h"

/**
 * 
 */
UCLASS()
class MYCONTENTEXAMPLE_API UMovementStateInstanceGround : public UMovementStateInstanceBase
{
	GENERATED_BODY()

public:
	UMovementStateInstanceGround();
	
	virtual void InputHandle(EMovementStateType MovementState) override;
	virtual UMovementStateInstanceBase* SwitchTargetStateInstance(EMovementStateType MovementState) override;

protected:
	virtual void StateChangedHandle(EMovementStateType MovementState) override;
};
