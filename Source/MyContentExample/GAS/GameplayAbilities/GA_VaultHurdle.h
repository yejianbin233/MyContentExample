// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/GameplayAbilities/GAS_GameplayAbility.h"
#include "GA_VaultHurdle.generated.h"

class UAbilityTask_PlayMontageAndWait;
/**
 * 
 */
UCLASS()
class MYCONTENTEXAMPLE_API UGA_VaultHurdle : public UGAS_GameplayAbility
{
	GENERATED_BODY()

	UGA_VaultHurdle();

	virtual bool CommitCheck(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, FGameplayTagContainer* OptionalRelevantTags) override;

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

protected:

	// 水平追踪球半径
	UPROPERTY(EditDefaultsOnly, Category="HorizontalTrace")
	float HorizontalTraceRadius = 30.0f;

	// 水平追踪总长度
	UPROPERTY(EditDefaultsOnly, Category="HorizontalTrace")
	float HorizontalTraceLength = 100.0f;

	// 水平追踪跨步距离
	UPROPERTY(EditDefaultsOnly, Category="HorizontalTrace")
	float VerticalTraceStartOffset = 30.0f;

	// 垂直追踪球半径
	UPROPERTY(EditDefaultsOnly, Category="VerticalTrace")
	float VerticalTraceRadius = 30.0f;

	// 垂直追踪高度
	UPROPERTY(EditDefaultsOnly, Category="VerticalTrace")
	float VerticalTraceHeight = 160.0f;
	
	// 垂直追踪最大高度（高于该高度不能跨栏）
	UPROPERTY(EditDefaultsOnly, Category="VerticalTrace")
	float VerticalTraceMaxHeight = 60.0f;
	
	// 垂直追踪最小高度（低于该高度不需要跨栏）
	UPROPERTY(EditDefaultsOnly, Category="VerticalTrace")
	float VerticalTraceMinHeight = 40.0f;

	// 追踪对象类型
	UPROPERTY(EditDefaultsOnly)
	TArray<TEnumAsByte<EObjectTypeQuery>> TraceObjectTypes;

	// 跨栏动作蒙太奇动画
	UPROPERTY(EditDefaultsOnly, Category="Montages")
	UAnimMontage* VaultHurdleMontage = nullptr;

	// 跨栏手部支撑位置
	FVector JumpOverLocation;

	// 跨栏终点位置（TODO 临时使用固定值）
	FVector JumpOverToLocation;

	// 跨栏终点位置偏移
	UPROPERTY(EditDefaultsOnly, Category="Offset")
	float JumpOverToLocationOffset = 40.0f;

	// 碰撞忽略通道
	UPROPERTY(EditDefaultsOnly)
	TArray<TEnumAsByte<ECollisionChannel>> CollsiionChannelsToIgnore;

	// 播放蒙太奇移动任务对象
	UAbilityTask_PlayMontageAndWait* MontageTask = nullptr;
};
