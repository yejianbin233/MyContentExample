// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Components/GAS_CharacterMovementComponent.h"

#include "AbilitySystemComponent.h"
#include "GAS/GameplayAbilities/GAS_GameplayAbility.h"


static TAutoConsoleVariable<int32> CVarShowTraversal(
	TEXT("ShowDebugTraversal"),
	0,
	TEXT("Draw debug info about traversal")
	TEXT("0: off/n")
	TEXT("1: on/n"),
	ECVF_Cheat);

bool UGAS_CharacterMovementComponent::TryTraversal(UAbilitySystemComponent* ASC)
{
	if (ASC)
	{
		for (int i = 0; i < TraversalAbilitiesOrdered.Num(); ++i)
		{
			TSubclassOf<UGameplayAbility> AbilityClass = TraversalAbilitiesOrdered[i];
			if (ASC->TryActivateAbilityByClass(AbilityClass, true))
			{
				FGameplayAbilitySpec* AbilitySpec;
		
				AbilitySpec = ASC->FindAbilitySpecFromClass(AbilityClass);
		
				if (AbilitySpec && AbilitySpec->IsActive())
				{
					return true;
				}
			}
		}
	}
	
	return false;
}
