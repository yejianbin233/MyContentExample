// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GAS_CharacterMovementComponent.generated.h"

class UGAS_GameplayAbility;
class UGameplayAbility;
class UAbilitySystemComponent;
/**
 * 
 */
UCLASS()
class MYCONTENTEXAMPLE_API UGAS_CharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()
	
public:

	// 尝试遍历，按照顺序激活运动，如：当靠近障碍物时使用了跳跃，那么此时跨越障碍物而不是单纯跳跃，如果跨越无法满足，将执行更次层级的运动。
	bool TryTraversal(UAbilitySystemComponent* ASC);

	
protected:

	// 运动技能执行遍历优先级数组。
	UPROPERTY(EditDefaultsOnly)
	TArray<TSubclassOf<UGameplayAbility>> TraversalAbilitiesOrdered;
};
