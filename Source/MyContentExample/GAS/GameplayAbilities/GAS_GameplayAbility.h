// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GAS_GameplayAbility.generated.h"

class AGAS_Character;
/**
 * 
 */
UCLASS()
class MYCONTENTEXAMPLE_API UGAS_GameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	
protected:

	// TODO Note：在技能对象中存储数据，需要注意技能的实例化方式。
	// 游戏效果数组，其游戏效果在技能生效时持续激活，当技能结束时被移除。
	UPROPERTY(EditDefaultsOnly, Category="GameplayEffect")
	TArray<TSubclassOf<UGameplayEffect>> OnGoingEffectsToRemoveOnEnd;

	// 游戏效果数组，其游戏效果在技能开始时激活
	UPROPERTY(EditDefaultsOnly, Category="GameplayEffect")
	TArray<TSubclassOf<UGameplayEffect>> OnGoingEffectsToJustApplyOnStart;

	TArray<FActiveGameplayEffectHandle> RemoveOnEndEffectHandles;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	AGAS_Character* GetGAS_CharacterFromActorInfo() const;
};
