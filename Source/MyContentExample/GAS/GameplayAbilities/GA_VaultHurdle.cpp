// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/GameplayAbilities/GA_VaultHurdle.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Kismet/KismetSystemLibrary.h"
#include "DrawDebugHelpers.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GAS/Character/GAS_Character.h"
#include "GAS/Components/GAS_MotionWarpingComponent.h"

UGA_VaultHurdle::UGA_VaultHurdle()
{
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

bool UGA_VaultHurdle::CommitCheck(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                  const FGameplayAbilityActivationInfo ActivationInfo, FGameplayTagContainer* OptionalRelevantTags)
{
	if (!Super::CommitCheck(Handle, ActorInfo, ActivationInfo, OptionalRelevantTags))
	{
		return false;
	}

	AGAS_Character* Character = GetGAS_CharacterFromActorInfo();

	if (!Character)
	{
		return false;
	}

	const FVector StartLocation = Character->GetActorLocation();
	const FVector ForwardVector = Character->GetActorForwardVector();
	const FVector UpVector = Character->GetActorUpVector();

	TArray<AActor*> ActorsToIgnore = {Character};

	static const auto CVar = IConsoleManager::Get().FindConsoleVariable(TEXT("ShowDebugTraversal"));
	const int32 bShowTraversal = CVar->GetInt();

	EDrawDebugTrace::Type DebugDrawType = bShowTraversal ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None;

	bool bJumpToLocationSet = false;

	int32 JumpToLocationIndex = INDEX_NONE;

	int i = 0;

	FHitResult TraceHitResult;

	float MaxJumpDistance = HorizontalTraceLength;

	for(; i < HorizontalTraceCount; ++i)
	{
		const FVector TraceStart = StartLocation + i * UpVector * HorizontalTraceStep;
		const FVector TraceEnd = TraceStart + ForwardVector * HorizontalTraceLength;

		if (UKismetSystemLibrary::SphereTraceSingleForObjects(this,
			TraceStart,
			TraceEnd,
			HorizontalTraceRadius,
			TraceObjectTypes,
			true,
			ActorsToIgnore,
			DebugDrawType,
			TraceHitResult,
			true))
		{
			if (JumpToLocationIndex == INDEX_NONE && (i < HorizontalTraceCount - 1))
			{
				JumpToLocationIndex = i;
				JumpToLocation = TraceHitResult.Location;
			}
			else if(JumpToLocationIndex == (i-1))
			{
				MaxJumpDistance = FVector::Dist2D(TraceHitResult.Location, TraceStart);
				break;
			}
		}
		else
		{
			if (JumpToLocationIndex != INDEX_NONE)
			{
				break;
			}
		}
	}

	if (JumpToLocationIndex == INDEX_NONE)
	{
		return false;
	}

	const float DistanceToJumpTo = FVector::Dist2D(StartLocation, JumpToLocation);

	const float MaxVerticalTraceDistance = MaxJumpDistance - DistanceToJumpTo;

	if (MaxVerticalTraceDistance < 0)
	{
		return false;
	}

	if (i == HorizontalTraceCount)
	{
		i = HorizontalTraceCount - 1;
	}

	const float VerticalTraceLength = FMath::Abs(JumpToLocation.Z - (StartLocation + i * UpVector * HorizontalTraceStep).Z);

	FVector VerticalStartLocation = JumpToLocation + UpVector * VerticalTraceLength;

	i = 0;

	const float VerticalTraceCount = MaxVerticalTraceDistance / VerticalTraceStep;

	bool bJumpOverLocationSet = false;

	for (; i <= VerticalTraceCount; ++i)
	{
		const FVector TraceStart = VerticalStartLocation + i * ForwardVector * VerticalTraceStep;

		const FVector TraceEnd = TraceStart + UpVector * VerticalTraceLength * -1;

		if (UKismetSystemLibrary::SphereTraceSingleForObjects(this,
			TraceStart,
			TraceEnd,
			HorizontalTraceRadius,
			TraceObjectTypes,
			true,
			ActorsToIgnore,
			DebugDrawType,
			TraceHitResult,
			true))
		{
			JumpOverLocation = TraceHitResult.ImpactPoint;
			if (i == 0)
			{
				JumpToLocation = JumpOverLocation;
			}
		}
		else if(i != 0)
		{
			bJumpOverLocationSet = true;
			break;
		}
	}

	if (!bJumpOverLocationSet)
	{
		return false;
	}

	const FVector TraceStart = JumpOverLocation + ForwardVector * VerticalTraceStep;
	if (UKismetSystemLibrary::SphereTraceSingleForObjects(this,
			TraceStart,
			JumpOverLocation,
			HorizontalTraceRadius,
			TraceObjectTypes,
			true,
			ActorsToIgnore,
			DebugDrawType,
			TraceHitResult,
			true))
	{
		JumpOverLocation = TraceHitResult.ImpactPoint;
	}

	if (bShowTraversal)
	{
		DrawDebugSphere(GetWorld(), JumpToLocation, 15, 16, FColor::White, false, 7);
		DrawDebugSphere(GetWorld(), JumpOverLocation, 15, 16, FColor::White, false, 7);
	}
	return true;
}

void UGA_VaultHurdle::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		K2_EndAbility();
		return;
	}

	AGAS_Character* Character = GetGAS_CharacterFromActorInfo();
	
	UGAS_CharacterMovementComponent* CharacterMovementComponent = Character ? Character->GetGasCharacterMovementComponent() : nullptr;

	if (CharacterMovementComponent)
	{
		CharacterMovementComponent->SetMovementMode(MOVE_Flying);
	}

	UCapsuleComponent* CapsuleComponent = Character ? Character->GetCapsuleComponent() : nullptr;
	
	if (CapsuleComponent)
	{
		for (ECollisionChannel Channel : CollsiionChannelsToIgnore)
		{
			CapsuleComponent->SetCollisionResponseToChannel(Channel, ECollisionResponse::ECR_Ignore);
		}
	}

	UGAS_MotionWarpingComponent* MotionWarpingComponent = Character ? Character->GetMotionWarpingComponent() : nullptr;
	if (MotionWarpingComponent)
	{
		MotionWarpingComponent->AddOrUpdateWarpTargetFromLocationAndRotation(TEXT("JumpToLocation"),
			JumpToLocation,
			Character->GetActorRotation());

		MotionWarpingComponent->AddOrUpdateWarpTargetFromLocationAndRotation(TEXT("JumpOverLocation"),
			JumpOverLocation,
			Character->GetActorRotation());
	}

	MontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, NAME_None, VaultHurdleMontage);

	MontageTask->OnBlendOut.AddDynamic(this, &UGA_VaultHurdle::K2_EndAbility);
	MontageTask->OnCompleted.AddDynamic(this, &UGA_VaultHurdle::K2_EndAbility);
	MontageTask->OnCancelled.AddDynamic(this, &UGA_VaultHurdle::K2_EndAbility);
	MontageTask->OnInterrupted.AddDynamic(this, &UGA_VaultHurdle::K2_EndAbility);
	// 激活任务
	MontageTask->ReadyForActivation();
}

void UGA_VaultHurdle::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	if (IsValid(MontageTask))
	{
		MontageTask->EndTask();
	}

	AGAS_Character* Character = GetGAS_CharacterFromActorInfo();
	if (Character)
	{
		UCapsuleComponent* CapsuleComponent = Character->GetCapsuleComponent();
		if (CapsuleComponent)
		{
			for (ECollisionChannel Channel : CollsiionChannelsToIgnore)
			{
				CapsuleComponent->SetCollisionResponseToChannel(Channel, ECollisionResponse::ECR_Block);
			}
		}
	}

	UGAS_CharacterMovementComponent* CharacterMovementComponent = Character ? Character->GetGasCharacterMovementComponent() : nullptr;
	
	if (CharacterMovementComponent && CharacterMovementComponent->IsFlying())
	{
		CharacterMovementComponent->SetMovementMode(MOVE_Falling);
	}


	UGAS_MotionWarpingComponent* MotionWarpingComponent = Character->GetMotionWarpingComponent();

	if (MotionWarpingComponent)
	{
		MotionWarpingComponent->RemoveWarpTarget(TEXT("JumpToLocation"));
		MotionWarpingComponent->RemoveWarpTarget(TEXT("JumpOverLocation"));
	}
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
