// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/AttributeSets/GAS_AttributeSetBase.h"
#include "GameplayEffectExtension.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Net/UnrealNetwork.h"

void UGAS_AttributeSetBase::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	// 判断当前修改的属性是否是 "Health"
	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		// 限制值在 [0, MaxHealth] 之间
		SetHealth(FMath::Clamp(GetHealth(), .0f, GetMaxHealth()));
	}
	else if (Data.EvaluatedData.Attribute == GetMaxMovementSpeedAttribute())
	{
		// 设置最大移动速度。
		SetHealth(FMath::Clamp(GetHealth(), .0f, GetMaxHealth()));
		ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwningActor());
		UCharacterMovementComponent* CharacterMovementComponent = OwnerCharacter ? OwnerCharacter->GetCharacterMovement() : nullptr;

		if (CharacterMovementComponent)
		{
			const float MaxSpeed = GetMaxMovementSpeed();
			CharacterMovementComponent->MaxWalkSpeed = MaxSpeed;
		}
	}
}

void UGAS_AttributeSetBase::OnRep_Health(const FGameplayAttributeData& OldHealth)
{
	// GAMEPLAYATTRIBUTE_REPNOTIFY 是一个帮助宏，可在 RepNotify 函数中用于处理将由客户端进行预测性修改的属性。
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGAS_AttributeSetBase, Health, OldHealth);
}

void UGAS_AttributeSetBase::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGAS_AttributeSetBase, MaxHealth, OldMaxHealth);
}

void UGAS_AttributeSetBase::OnRep_Stamina(const FGameplayAttributeData& OldStamina)
{
	// GAMEPLAYATTRIBUTE_REPNOTIFY 是一个帮助宏，可在 RepNotify 函数中用于处理将由客户端进行预测性修改的属性。
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGAS_AttributeSetBase, Stamina, OldStamina);
}

void UGAS_AttributeSetBase::OnRep_MaxStamina(const FGameplayAttributeData& OldMaxStamina)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGAS_AttributeSetBase, MaxStamina, OldMaxStamina);
}

void UGAS_AttributeSetBase::OnRep_MaxMovementSpeed(const FGameplayAttributeData& OldMaxMovementSpeed)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGAS_AttributeSetBase, MaxMovementSpeed, OldMaxMovementSpeed);
}

void UGAS_AttributeSetBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UGAS_AttributeSetBase, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGAS_AttributeSetBase, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGAS_AttributeSetBase, Stamina, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGAS_AttributeSetBase, MaxStamina, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGAS_AttributeSetBase, MaxMovementSpeed, COND_None, REPNOTIFY_Always);
}
