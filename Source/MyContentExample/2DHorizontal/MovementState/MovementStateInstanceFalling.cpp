// Fill out your copyright notice in the Description page of Project Settings.


#include "2DHorizontal/MovementState/MovementStateInstanceFalling.h"

#include "MovementStateInstanceGround.h"
#include "MovementStateInstanceLanded.h"
#include "2DHorizontal/GameDatas/MovementType.h"

UMovementStateInstanceFalling::UMovementStateInstanceFalling()
{
	MovementMode = EHorizontal2DMovementMode::Hor2D_Falling;
}

void UMovementStateInstanceFalling::InputHandle(EMovementStateType MovementState)
{
	Super::InputHandle(MovementState);
}

UMovementStateInstanceBase* UMovementStateInstanceFalling::SwitchTargetStateInstance(EMovementStateType MovementState)
{
	switch (MovementState)
	{
		case EMovementStateType::OnLanded:
			return NewObject<UMovementStateInstanceLanded>();
		default: ;
	}

	return this;
}

void UMovementStateInstanceFalling::StateChangedHandle(EMovementStateType MovementState)
{
	Super::StateChangedHandle(MovementState);
}
