// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MotionWarpingComponent.h"
#include "GAS_MotionWarpingComponent.generated.h"

/**
 * 
 */
UCLASS()
class MYCONTENTEXAMPLE_API UGAS_MotionWarpingComponent : public UMotionWarpingComponent
{
	GENERATED_BODY()

	UGAS_MotionWarpingComponent(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastSyncWarpPoints(const TArray<FMotionWarpingTarget>& Targets);

public:
	void SendWarpPointsToClient();
};
