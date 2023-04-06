// Fill out your copyright notice in the Description page of Project Settings.


#include "2DHorizontal/MovementState/MovementStateInstanceGround.h"

#include "MovementStateInstanceFalling.h"
#include "MovementStateInstanceJump.h"
#include "2DHorizontal/Components/Hori2DCharacterMovementComponent.h"
#include "2DHorizontal/GameDatas/MovementType.h"
#include "GameFramework/Character.h"

UMovementStateInstanceGround::UMovementStateInstanceGround()
{
	MovementMode = EHorizontal2DMovementMode::Hor2D_MOVE_GroundWalking;
}

void UMovementStateInstanceGround::InputHandle(EMovementStateType MovementState)
{
	Super::InputHandle(MovementState);

	
}

UMovementStateInstanceBase* UMovementStateInstanceGround::SwitchTargetStateInstance(EMovementStateType MovementState)
{
	switch (MovementState)
	{
		case EMovementStateType::OnJumping:
			return NewObject<UMovementStateInstanceJump>();
		case EMovementStateType::OnFalling:
			return NewObject<UMovementStateInstanceFalling>();
		default: ;
	}

	return this;
}

void UMovementStateInstanceGround::StateChangedHandle(EMovementStateType MovementState)
{
	Super::StateChangedHandle(MovementState);
}
