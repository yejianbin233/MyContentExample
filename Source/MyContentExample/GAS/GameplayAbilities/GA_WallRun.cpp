// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/GameplayAbilities/GA_WallRun.h"

#include "AbilitySystemComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"
#include "GAS/AbilityTasks/AbilityTask_TickWallRun.h"
#include "GAS/Character/GAS_Character.h"

UGA_WallRun::UGA_WallRun()
{
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UGA_WallRun::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnGiveAbility(ActorInfo, Spec);

	// 在授予技能时开始绑定 Character 的胶囊体碰撞事件
	ACharacter* Character = Cast<ACharacter>(GetGAS_CharacterFromActorInfo());
	UCapsuleComponent* CapsuleComponent = Character->GetCapsuleComponent();

	CapsuleComponent->OnComponentHit.AddDynamic(this, &UGA_WallRun::OnCapsuleComponentHit);
	
}

void UGA_WallRun::OnRemoveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	if (ActorInfo)
	{
		ACharacter* Character = Cast<ACharacter>(GetGAS_CharacterFromActorInfo());
		UCapsuleComponent* CapsuleComponent = Character->GetCapsuleComponent();

		CapsuleComponent->OnComponentHit.RemoveDynamic(this, &UGA_WallRun::OnCapsuleComponentHit);
	}
	
	Super::OnRemoveAbility(ActorInfo, Spec);
	// 在移除技能时，接触 Character 胶囊体的碰撞响应事件
}

bool UGA_WallRun::CanActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags,
	const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	if (!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
	{
		return false;
	}

	AGAS_Character* Character = GetGAS_CharacterFromActorInfo();
	// 只有在地面行走时才允许墙壁行走。
	return Character && Character->GetGasCharacterMovementComponent()->IsMovingOnGround();
}

void UGA_WallRun::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	// 技能激活时，执行技能任务。
	WallRunTask = UAbilityTask_TickWallRun::CreateWallRunTask(this,
		Cast<ACharacter>(GetGAS_CharacterFromActorInfo()),
		Cast<UCharacterMovementComponent>(GetGAS_CharacterFromActorInfo()->GetGasCharacterMovementComponent()),
		WallRunTraceObjectTypes);

	WallRunTask->OnFinished.AddDynamic(this, &UGA_WallRun::K2_EndAbility);
	WallRunTask->OnWallSideDeternened.AddDynamic(this, &UGA_WallRun::OnWallSideDeternened);

	WallRunTask->ReadyForActivation();
	
}

void UGA_WallRun::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	if (IsValid(WallRunTask))
	{
		WallRunTask->EndTask();
	}

	UAbilitySystemComponent* AbilitySystemComponent = GetAbilitySystemComponentFromActorInfo();

	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->RemoveActiveGameplayEffectBySourceEffect(WallRunLeftSideEffectClass, AbilitySystemComponent);
		AbilitySystemComponent->RemoveActiveGameplayEffectBySourceEffect(WallRunRightSideEffectClass, AbilitySystemComponent);
	}
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
	
}

void UGA_WallRun::OnCapsuleComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& HitResult)
{
	UAbilitySystemComponent* AbilitySystemComponent = GetAbilitySystemComponentFromActorInfo();
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->TryActivateAbility(GetCurrentAbilitySpec()->Handle);
	}
}

void UGA_WallRun::OnWallSideDeternened(bool bLeftSide)
{
	AGAS_Character* Character = GetGAS_CharacterFromActorInfo();

	UAbilitySystemComponent* AbilitySystemComponent = GetAbilitySystemComponentFromActorInfo();

	FGameplayEffectContextHandle GameplayEffectContextHandle = AbilitySystemComponent->MakeEffectContext();

	if (bLeftSide)
	{
		Character->ApplyGameplayEffecToSelf(WallRunLeftSideEffectClass, GameplayEffectContextHandle);
	}
	else
	{
		Character->ApplyGameplayEffecToSelf(WallRunRightSideEffectClass, GameplayEffectContextHandle);
	}
}
