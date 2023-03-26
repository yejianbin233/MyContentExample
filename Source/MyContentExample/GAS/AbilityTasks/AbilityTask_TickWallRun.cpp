// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/AbilityTasks/AbilityTask_TickWallRun.h"

#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"

UAbilityTask_TickWallRun* UAbilityTask_TickWallRun::CreateWallRunTask(UGameplayAbility* OwningAbility,
                                                                      ACharacter* InCharacterOwner,
                                                                      UCharacterMovementComponent* InCharacterMovementComponent,
                                                                      TArray<TEnumAsByte<EObjectTypeQuery>> InTraceObjectTypes)
{
	UAbilityTask_TickWallRun* WallRunTask = NewAbilityTask<UAbilityTask_TickWallRun>(OwningAbility);

	WallRunTask->CharacterOwner = InCharacterOwner;
	WallRunTask->CharacterMovementComponent = InCharacterMovementComponent;
	WallRunTask->TraceObjectTypes = InTraceObjectTypes;

	return WallRunTask;
}

void UAbilityTask_TickWallRun::Activate()
{
	Super::Activate();

	bool IsShowDebugInfo = true;
	FHitResult OnWallHitResult;

	const FVector CurrentAcceleration = CharacterMovementComponent->GetCurrentAcceleration();
	
	if (!FindRunnableWall(OnWallHitResult))
	{
		if (IsShowDebugInfo)
		{
			GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Green, FString(TEXT("没有找到可行走的墙壁,结束任务!")));
		}
		
		if (ShouldBroadcastAbilityTaskDelegates())
		{
			OnFinished.Broadcast();
		}
		EndTask();
		return;
	}

	bool bLeftSide = IsWallOnTheLeft(OnWallHitResult);
	OnWallSideDeternened.Broadcast(bLeftSide);
	if (IsShowDebugInfo)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Green, FString::Printf(TEXT("找到可行走的墙壁, Q: 是否是左边墙壁可行走? A: %s."), bLeftSide ? TEXT("是") : TEXT("否")));
	}

	// 特殊处理，结束所有空中状态
	CharacterOwner->Landed(OnWallHitResult);

	// 改变位置, TODO 使用魔法值，后续再优化
	CharacterOwner->SetActorLocation(OnWallHitResult.ImpactPoint + OnWallHitResult.ImpactNormal * 60.0f);

	// 设置移动模式为飞行模式，可在空中滞空运动
	CharacterMovementComponent->SetMovementMode(MOVE_Flying);

	
}

void UAbilityTask_TickWallRun::OnDestroy(bool bInOwnerFinished)
{
	// TODO ??? 什么作用?
	CharacterMovementComponent->SetPlaneConstraintEnabled(false);
	CharacterMovementComponent->SetMovementMode(MOVE_Falling);

	Super::OnDestroy(bInOwnerFinished);
}

void UAbilityTask_TickWallRun::TickTask(float DeltaTime)
{
	Super::TickTask(DeltaTime);

	bool IsShowDebugInfo = true;

	FHitResult OnWallHitResult;

	const FVector CurrentAcceleration = CharacterMovementComponent->GetCurrentAcceleration();

	if (!FindRunnableWall(OnWallHitResult))
	{
		if (ShouldBroadcastAbilityTaskDelegates())
		{
			if (IsShowDebugInfo)
			{
				GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Green, FString(TEXT("到达可行走墙壁的终点,结束该次墙壁行走运动.")));
			}

			OnFinished.Broadcast();
			EndTask();
		}
	}

	const FRotator DirectionRotator = IsWallOnTheLeft(OnWallHitResult) ? FRotator(0, -90, 0) : FRotator(0, 90, 0);

	// TODO 需要绘制这些方向的线条，便于调试。
	const FVector WallRunDirection = DirectionRotator.RotateVector(OnWallHitResult.ImpactNormal);

	CharacterMovementComponent->Velocity = WallRunDirection * 700.0f;

	// 设置下落速度，受重力影响。
	CharacterMovementComponent->Velocity.Z = CharacterMovementComponent->GetGravityZ() * DeltaTime;

	CharacterOwner->SetActorRotation(WallRunDirection.Rotation());

	CharacterMovementComponent->SetPlaneConstraintEnabled(true);
	
	// 设置平面约束。
	CharacterMovementComponent->SetPlaneConstraintOrigin(OnWallHitResult.ImpactPoint);
	CharacterMovementComponent->SetPlaneConstraintNormal(OnWallHitResult.ImpactNormal);
}

bool UAbilityTask_TickWallRun::FindRunnableWall(FHitResult& OnWallHitResult)
{
	const FVector CharacterLocation = CharacterOwner->GetActorLocation();
	const FVector ForwardVector = CharacterOwner->GetActorForwardVector();
	const FVector RightVector = CharacterOwner->GetActorRightVector();

	const float TraceLength = CharacterOwner->GetCapsuleComponent()->GetScaledCapsuleRadius() + 30.0f;

	TArray<AActor*> ActorsToIgnore = {CharacterOwner};

	FHitResult TraceHitResult;
	
	static const auto CVar = IConsoleManager::Get().FindConsoleVariable(TEXT("ShowDebugTraversal"));
	const bool IsShowDebugInfo = CVar ? false : true;
	EDrawDebugTrace::Type  DebugDrawType = IsShowDebugInfo ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None;

	if (UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(),
		CharacterLocation,
		CharacterLocation + ForwardVector * TraceLength,
		TraceObjectTypes,
		true,
		ActorsToIgnore,
		DebugDrawType,
		OnWallHitResult, true))
	{
		return false;
	}
	
	if (UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(),
			CharacterLocation,
			CharacterLocation + (-RightVector) * TraceLength,
			TraceObjectTypes,
			true,
			ActorsToIgnore,
			DebugDrawType,
			OnWallHitResult, true))
	{
		if (FVector::DotProduct(OnWallHitResult.ImpactNormal, RightVector) > 0.30f)
		{
			return true;
		}
	}

	if (UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(),
			CharacterLocation,
			CharacterLocation + (-RightVector) * TraceLength,
			TraceObjectTypes,
			true,
			ActorsToIgnore,
			DebugDrawType,
			OnWallHitResult, true))
	{
		if (FVector::DotProduct(OnWallHitResult.ImpactNormal, -RightVector) > 0.30f)
		{
			return true;
		}
	}

	return false;
}

bool UAbilityTask_TickWallRun::IsWallOnTheLeft(const FHitResult& InWallHitResult) const
{
	return FVector::DotProduct(CharacterOwner->GetActorRightVector(), InWallHitResult.ImpactNormal) > 0.0f;
}
