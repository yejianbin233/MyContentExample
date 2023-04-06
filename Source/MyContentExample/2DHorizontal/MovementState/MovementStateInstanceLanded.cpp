// Fill out your copyright notice in the Description page of Project Settings.


#include "2DHorizontal/MovementState/MovementStateInstanceLanded.h"

#include "MovementStateInstanceGround.h"
#include "MovementStateInstanceJump.h"
#include "2DHorizontal/GameDatas/MovementType.h"

UMovementStateInstanceLanded::UMovementStateInstanceLanded()
{
	MovementMode = EHorizontal2DMovementMode::Hor2D_Landed;
}

void UMovementStateInstanceLanded::InputHandle(EMovementStateType MovementState)
{
	Super::InputHandle(MovementState);
}

UMovementStateInstanceBase* UMovementStateInstanceLanded::SwitchTargetStateInstance(EMovementStateType MovementState)
{
	switch (MovementState)
	{
		case EMovementStateType::OnMove:
			return NewObject<UMovementStateInstanceGround>();
		case EMovementStateType::OnGroundWalking:
			return NewObject<UMovementStateInstanceGround>();
		case EMovementStateType::OnJumping:
			return NewObject<UMovementStateInstanceJump>();
		case EMovementStateType::OnGroundIdle:
			return NewObject<UMovementStateInstanceGround>();
		default: ;
	}

	return this;
}

void UMovementStateInstanceLanded::StateChangedHandle(EMovementStateType MovementState)
{
	Super::StateChangedHandle(MovementState);
}
