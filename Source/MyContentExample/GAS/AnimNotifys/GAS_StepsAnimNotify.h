// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "GAS/GameType/GAS_GameTypes.h"
#include "GAS_StepsAnimNotify.generated.h"

/**
 * TODO 使用动画修改器来自动添加动画通知（AnimNotify），而不是手动。
 */
UCLASS()
class MYCONTENTEXAMPLE_API UGAS_StepsAnimNotify : public UAnimNotify
{
	GENERATED_BODY()

public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

	UPROPERTY(EditAnywhere)
	EFoot Foot;
};
