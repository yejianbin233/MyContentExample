// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "GAS_AnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class MYCONTENTEXAMPLE_API UGAS_AnimInstance : public UAnimInstance
{
	GENERATED_BODY()

protected:
	UFUNCTION(BlueprintCallable, meta=(BlueprintThreadSafe))
	class UBlendSpace* GetLocomotionBlendSpace() const;
	
	UFUNCTION(BlueprintCallable, meta=(BlueprintThreadSafe))
	class UAnimSequenceBase* GetIdleAnimAsset() const;

	UFUNCTION(BlueprintCallable, meta=(BlueprintThreadSafe))
	UBlendSpace* GetCrouchLocomotionBlendSpace() const;
	
	UFUNCTION(BlueprintCallable, meta=(BlueprintThreadSafe))
	UAnimSequenceBase* GetCrouchIdleAnimAsset() const;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="AnimDataAsset", DisplayName="默认动画资产")
	class UGAS_CharacterAnimDataAsset* DefaultCharacterAnimDataAsset;
};
