// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/GameplayAbilities/GAS_GameplayAbility.h"
#include "GA_WallRun.generated.h"

class UAbilityTask_TickWallRun;
/**
 * 在激活墙壁行走技能后，动画蓝图将根据技能所设置的状态来切换行走动画。
 */
UCLASS()
class MYCONTENTEXAMPLE_API UGA_WallRun : public UGAS_GameplayAbility
{
	GENERATED_BODY()

	UGA_WallRun();

public:

	virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;

	virtual void OnRemoveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;

	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const override;

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

protected:

	UFUNCTION()
	void OnCapsuleComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& HitResult);

	UFUNCTION()
	void OnWallSideDeternened(bool bLeftSide);

	UPROPERTY(EditDefaultsOnly)
	UAbilityTask_TickWallRun* WallRunTask = nullptr;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayEffect> WallRunLeftSideEffectClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayEffect> WallRunRightSideEffectClass;

	UPROPERTY(EditDefaultsOnly)
	TArray<TEnumAsByte<EObjectTypeQuery>> WallRunTraceObjectTypes;
};
