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

	EDrawDebugTrace::Type DebugDrawType = bShowTraversal ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::Persistent;
	// EDrawDebugTrace::Type DebugDrawType = EDrawDebugTrace::Persistent;

	const FVector HorizontalTraceStart = StartLocation;
	const FVector HorizontalTraceEnd = HorizontalTraceStart + ForwardVector * HorizontalTraceLength;
	FHitResult TraceHitResult;

	FVector ObstacleLocation;

	if (UKismetSystemLibrary::SphereTraceSingleForObjects(this,
		HorizontalTraceStart,
		HorizontalTraceEnd,
		HorizontalTraceRadius,
		TraceObjectTypes,
		true,
		ActorsToIgnore,
		DebugDrawType,
		TraceHitResult,
		true))
	{
		ObstacleLocation = TraceHitResult.ImpactPoint;
	}
	else
	{
		return false;
	}

	const FVector VerticalTraceStart = ObstacleLocation + (ForwardVector * VerticalTraceStartOffset) + (UpVector * VerticalTraceHeight);
	const FVector VerticalTraceEnd = FVector(VerticalTraceStart.X, VerticalTraceStart.Y, HorizontalTraceStart.Z + VerticalTraceMinHeight);
	
	if (UKismetSystemLibrary::SphereTraceSingleForObjects(this,
		VerticalTraceStart,
		VerticalTraceEnd,
		HorizontalTraceRadius,
		TraceObjectTypes,
		true,
		ActorsToIgnore,
		DebugDrawType,
		TraceHitResult,
		true))
	{
		
		if (JumpOverLocation.Z > (HorizontalTraceStart.Z + VerticalTraceMaxHeight))
		{
			if (bShowTraversal)
			{
				GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Red, FString(TEXT("跨越点高度已超过约定值！")));
			}
			return false;
		}
		JumpOverLocation = TraceHitResult.ImpactPoint;

		const FVector JumpOverToLocationTraceStart = JumpOverLocation + ForwardVector * JumpOverToLocationOffset;;
		const FVector JumpOverToLocationTraceEnd = JumpOverToLocationTraceStart + (-UpVector * 150);
		
		if (UKismetSystemLibrary::SphereTraceSingleForObjects(this,
			JumpOverToLocationTraceStart,
			JumpOverToLocationTraceEnd,
			HorizontalTraceRadius,
			TraceObjectTypes,
			true,
			ActorsToIgnore,
			DebugDrawType,
			TraceHitResult,
			true))
		{
			JumpOverToLocation = TraceHitResult.ImpactPoint;
		}
		else
		{
			JumpOverToLocation = JumpOverToLocationTraceEnd;
		}
	}
	else
	{
		return false;
	}

	if (bShowTraversal)
	{
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
		CharacterMovementComponent->SetMovementMode(EMovementMode::MOVE_Flying);
	}

	UCapsuleComponent* CapsuleComponent = Character ? Character->GetCapsuleComponent() : nullptr;
	
	if (CapsuleComponent)
	{
		for (ECollisionChannel Channel : CollsiionChannelsToIgnore)
		{
			CapsuleComponent->SetCollisionResponseToChannel(Channel, ECollisionResponse::ECR_Ignore);
		}
	}

	// UGAS_MotionWarpingComponent* MotionWarpingComponent = Character ? Character->GetMotionWarpingComponent() : nullptr;
	// if (MotionWarpingComponent)
	// {
	// 	MotionWarpingComponent->AddOrUpdateWarpTargetFromLocationAndRotation(TEXT("JumpOverLocation"),
	// 		JumpOverLocation,
	// 		Character->GetActorRotation());
	//
	// 	MotionWarpingComponent->AddOrUpdateWarpTargetFromLocationAndRotation(TEXT("JumpOverToLocation"),
	// 		JumpOverToLocation,
	// 		Character->GetActorRotation());
	//
	// 	MotionWarpingComponent->SendWarpPointsToClient();
	// }

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
	if (MontageTask)
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
		CharacterMovementComponent->SetMovementMode(EMovementMode::MOVE_Walking);
	}


	// UGAS_MotionWarpingComponent* MotionWarpingComponent = Character->GetMotionWarpingComponent();
	//
	// if (MotionWarpingComponent)
	// {
	// 	MotionWarpingComponent->RemoveWarpTarget(TEXT("JumpOverLocation"));
	// 	MotionWarpingComponent->RemoveWarpTarget(TEXT("JumpOverToLocation"));
	// }
	//
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
