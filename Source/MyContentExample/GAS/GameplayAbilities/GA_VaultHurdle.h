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

	UPROPERTY(EditDefaultsOnly, Category="HorizontalTrace")
	float HorizontalTraceRadius = 30.0f;

	UPROPERTY(EditDefaultsOnly, Category="HorizontalTrace")
	float HorizontalTraceLength = 500.0f;

	UPROPERTY(EditDefaultsOnly, Category="HorizontalTrace")
	float HorizontalTraceCount = 5.0f;

	UPROPERTY(EditDefaultsOnly, Category="HorizontalTrace")
	float HorizontalTraceStep = 30.0f;

	UPROPERTY(EditDefaultsOnly, Category="VerticalTrace")
	float VerticalTraceRadius = 30.0f;

	UPROPERTY(EditDefaultsOnly, Category="VerticalTrace")
	float VerticalTraceStep = 30.0f;

	UPROPERTY(EditDefaultsOnly)
	TArray<TEnumAsByte<EObjectTypeQuery>> TraceObjectTypes;

	UPROPERTY(EditDefaultsOnly, Category="Montages")
	UAnimMontage* VaultHurdleMontage = nullptr;

	FVector JumpToLocation;

	FVector JumpOverLocation;

	UPROPERTY(EditDefaultsOnly)
	TArray<TEnumAsByte<ECollisionChannel>> CollsiionChannelsToIgnore;

	// 播放蒙太奇移动任务对象
	UAbilityTask_PlayMontageAndWait* MontageTask = nullptr;
};
