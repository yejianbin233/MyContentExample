// Fill out your copyright notice in the Description page of Project Settings.


#include "2DHorizontal/MovementState/MovementStateInstanceBase.h"

UMovementStateInstanceBase::UMovementStateInstanceBase()
{
	
}

void UMovementStateInstanceBase::InputHandle(EMovementStateType MovementState)
{
	MovementStateType = MovementState;
}

UMovementStateInstanceBase* UMovementStateInstanceBase::SwitchTargetStateInstance(EMovementStateType MovementState)
{
	StateChangedHandle(MovementState);
	return NewObject<UMovementStateInstanceBase>();
}

void UMovementStateInstanceBase::StateChangedHandle(EMovementStateType MovementState)
{
	
}