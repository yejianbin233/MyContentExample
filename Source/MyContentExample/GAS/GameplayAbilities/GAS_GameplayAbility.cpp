// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/GameplayAbilities/GAS_GameplayAbility.h"

#include "AbilitySystemComponent.h"
#include "GAS/Character/GAS_Character.h"

void UGAS_GameplayAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                           const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                           const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	UAbilitySystemComponent* AbilitySystemComponent = ActorInfo->AbilitySystemComponent.Get();

	FGameplayEffectContextHandle EffectContextHandle = AbilitySystemComponent->MakeEffectContext();

	for (auto GameplayEffect : OnGoingEffectsToJustApplyOnStart)
	{
		if (!GameplayEffect.Get()) continue;

		FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(GameplayEffect, 1, EffectContextHandle);

		if (SpecHandle.IsValid())
		{
			FActiveGameplayEffectHandle ActiveGameplayEffect = AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());

			if (!ActiveGameplayEffect.WasSuccessfullyApplied())
			{
				UE_LOG(LogTemp, Log, TEXT("Ability %s failed to apply startup effect %s"), *GetName(), *GetNameSafe(GameplayEffect));
			}
		}
	}

	// 判断技能是否是实例化的
	if (IsInstantiated())
	{
		for (auto EffectsToRemoveOnEnd : OnGoingEffectsToRemoveOnEnd)
		{
			FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(EffectsToRemoveOnEnd, 1, EffectContextHandle);

			if (SpecHandle.IsValid())
			{
				FActiveGameplayEffectHandle ActiveGameplayEffectHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());

				if (!ActiveGameplayEffectHandle.WasSuccessfullyApplied())
				{
					UE_LOG(LogTemp, Log, TEXT("Ability %s failed to apply startup effect %s"), *GetName(), *GetNameSafe(EffectsToRemoveOnEnd));
				}
				else
				{
					RemoveOnEndEffectHandles.Add(ActiveGameplayEffectHandle);
				}
			}
		}
	}
}

void UGAS_GameplayAbility::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	UAbilitySystemComponent* AbilitySystemComponent = ActorInfo->AbilitySystemComponent.Get();
	if (AbilitySystemComponent)
	{
		if (IsInstantiated())
		{
			for (auto ActiveGameplayEffectHandle : RemoveOnEndEffectHandles)
			{
				if (ActiveGameplayEffectHandle.IsValid())
				{
					ActorInfo->AbilitySystemComponent->RemoveActiveGameplayEffect(ActiveGameplayEffectHandle);
				}
			}

			RemoveOnEndEffectHandles.Empty();
		}
	}
}

AGAS_Character* UGAS_GameplayAbility::GetGAS_CharacterFromActorInfo() const
{
	return Cast<AGAS_Character>(GetAvatarActorFromActorInfo());
}
