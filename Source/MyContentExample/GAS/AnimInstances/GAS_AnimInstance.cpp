// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/AnimInstances/GAS_AnimInstance.h"

#include "GAS/Character/GAS_Character.h"

UBlendSpace* UGAS_AnimInstance::GetLocomotionBlendSpace() const
{
	AGAS_Character* Character = Cast<AGAS_Character>(GetOwningActor());
	if (Character)
	{
		const UGAS_CharacterAnimDataAsset* CharacterAnimDate = Character->GetGASCharacterAnimData();

		return CharacterAnimDate->CharacterAnimData.MovementBlendspace;
	}
	
	return DefaultCharacterAnimDataAsset->CharacterAnimData.MovementBlendspace;
}

UAnimSequenceBase* UGAS_AnimInstance::GetIdleAnimAsset() const
{
	AGAS_Character* Character = Cast<AGAS_Character>(GetOwningActor());
	if (Character)
	{
		const UGAS_CharacterAnimDataAsset* CharacterAnimDate = Character->GetGASCharacterAnimData();

		return CharacterAnimDate->CharacterAnimData.IdleAnimationAsset;
	}

	return DefaultCharacterAnimDataAsset->CharacterAnimData.IdleAnimationAsset;
}

UBlendSpace* UGAS_AnimInstance::GetCrouchLocomotionBlendSpace() const
{
	AGAS_Character* Character = Cast<AGAS_Character>(GetOwningActor());
	if (Character)
	{
		const UGAS_CharacterAnimDataAsset* CharacterAnimDate = Character->GetGASCharacterAnimData();

		return CharacterAnimDate->CharacterAnimData.CrouchMovementBlendspace;
	}
	
	return DefaultCharacterAnimDataAsset->CharacterAnimData.MovementBlendspace;
}

UAnimSequenceBase* UGAS_AnimInstance::GetCrouchIdleAnimAsset() const
{
	AGAS_Character* Character = Cast<AGAS_Character>(GetOwningActor());
	if (Character)
	{
		const UGAS_CharacterAnimDataAsset* CharacterAnimDate = Character->GetGASCharacterAnimData();

		return CharacterAnimDate->CharacterAnimData.CrouchIdleAnimationAsset;
	}

	return DefaultCharacterAnimDataAsset->CharacterAnimData.CrouchIdleAnimationAsset;
}
