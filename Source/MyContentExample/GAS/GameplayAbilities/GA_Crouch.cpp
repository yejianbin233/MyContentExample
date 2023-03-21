 // Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/GameplayAbilities/GA_Crouch.h"

#include "GameFramework/Character.h"

 UGA_Crouch::UGA_Crouch()
 {
 	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
 	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
 }

 bool UGA_Crouch::CanActivateAbility(const FGameplayAbilitySpecHandle Handle,
	 const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags,
	 const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
 {

	 if(!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
	 {
		 return false;
	 }

 	 const ACharacter* Character = CastChecked<ACharacter>(ActorInfo->AvatarActor.Get(), ECastCheckedType::NullAllowed);
	 if (Character)
	 {
		 return Character->CanCrouch();
	 }
	 else
	 {
		 return false;
	 }
 	
 }

 void UGA_Crouch::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                  const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
 {
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

 	ACharacter* Character = CastChecked<ACharacter>(ActorInfo->AvatarActor.Get(), ECastCheckedType::NullAllowed);

	if (Character)
	{
		Character->Crouch();
	}
 }

 void UGA_Crouch::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
  const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
 {
 	ACharacter* Character = CastChecked<ACharacter>(ActorInfo->AvatarActor.Get(), ECastCheckedType::NullAllowed);

 	if (Character)
 	{
 		Character->UnCrouch();
 	}
 	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
 }
