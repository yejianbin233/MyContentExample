// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Components/GAS_MotionWarpingComponent.h"
#include "GAS/GameType/GAS_GameTypes.h"

UGAS_MotionWarpingComponent::UGAS_MotionWarpingComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	SetIsReplicated(true);
}

void UGAS_MotionWarpingComponent::MulticastSyncWarpPoints_Implementation(const TArray<FMotionWarpingTarget>& Targets)
{
	for (auto Target : Targets)
	{
		AddOrUpdateWarpTargetFromLocationAndRotation(Target.Name, Target.Location, FRotator(Target.Rotation));
	}
}

void UGAS_MotionWarpingComponent::SendWarpPointsToClient()
{
	if (GetOwnerRole() == ROLE_Authority)
	{
		MulticastSyncWarpPoints(WarpTargets);
	}
}


