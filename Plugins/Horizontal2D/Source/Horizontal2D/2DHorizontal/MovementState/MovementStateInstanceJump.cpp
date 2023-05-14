// Fill out your copyright notice in the Description page of Project Settings.


#include "2DHorizontal/MovementState/MovementStateInstanceJump.h"

#include "MovementStateInstanceFalling.h"
#include "MovementStateInstanceGround.h"
#include "2DHorizontal/GameDatas/MovementType.h"
#include "2DHorizontal/Components/Hori2DCharacterMovementComponent.h"
#include "GameFramework/Character.h"

UMovementStateInstanceJump::UMovementStateInstanceJump()
{
	MovementMode = EHorizontal2DMovementMode::Hor2D_Jumping;
}

void UMovementStateInstanceJump::InputHandle(EMovementStateType MovementState)
{
	Super::InputHandle(MovementState);

	if (MovementStateType == EMovementStateType::OnJumping)
	{
		ACharacter* Character = Cast<ACharacter>(Owner->GetOwner());

		if (Character)
		{
			Character->Jump();
		}
	}
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
		case EMovementStateType::OnFalling:
			return NewObject<UMovementStateInstanceFalling>();
		default: ;
	}

	return this;
}
