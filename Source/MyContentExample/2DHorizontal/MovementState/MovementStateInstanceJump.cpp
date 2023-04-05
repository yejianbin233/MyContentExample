// Fill out your copyright notice in the Description page of Project Settings.


#include "2DHorizontal/MovementState/MovementStateInstanceJump.h"

#include "MovementStateInstanceGround.h"
#include "2DHorizontal/GameDatas/MovementType.h"

UMovementStateInstanceJump::UMovementStateInstanceJump()
{
	MovementMode = EHorizontal2DMovementMode::Hor2D_MOVE_Jumping;
}

void UMovementStateInstanceJump::InputHandle(EMovementStateType MovementState)
{
	Super::InputHandle(MovementState);
}

void UMovementStateInstanceJump::StateChangedHandle(EMovementStateType MovementState)
{
	Super::StateChangedHandle(MovementState);
}

UMovementStateInstanceBase* UMovementStateInstanceJump::SwitchTargetStateInstance(EMovementStateType MovementState)
{
	switch (MovementState)
	{
		case EMovementStateType::OnLanded:
			return NewObject<UMovementStateInstanceGround>();
		default: ;
	}

	return this;
}
