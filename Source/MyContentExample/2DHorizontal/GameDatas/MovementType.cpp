// Fill out your copyright notice in the Description page of Project Settings.


#include "MovementType.h"

UMovementStateInstanceBase* UMovementStateInstanceBase::InputHandle(EMovementStateType MovementState)
{
	return NewObject<UMovementStateInstanceBase>();	
}

bool UMovementStateInstanceBase::SwitchTargetState(EMovementStateType MovementState)
{
	StateChangedHandle(MovementState);

	return true;
}

void UMovementStateInstanceBase::StateChangedHandle(EMovementStateType MovementState)
{
	
}
