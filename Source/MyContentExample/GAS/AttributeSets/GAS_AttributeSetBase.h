// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "GAS_AttributeSetBase.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

/**
 * 
 */
UCLASS()
class MYCONTENTEXAMPLE_API UGAS_AttributeSetBase : public UAttributeSet
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, Category="AttributeSet", ReplicatedUsing = OnRep_Health, DisplayName="生命值")
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UGAS_AttributeSetBase, Health);

	UPROPERTY(BlueprintReadOnly, Category="AttributeSet", ReplicatedUsing = OnRep_MaxHealth, DisplayName="最大生命值")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UGAS_AttributeSetBase, MaxHealth);

	UPROPERTY(BlueprintReadOnly, Category="AttributeSet", ReplicatedUsing = OnRep_Stamina, DisplayName="体力")
	FGameplayAttributeData Stamina;
	ATTRIBUTE_ACCESSORS(UGAS_AttributeSetBase, Stamina);

	UPROPERTY(BlueprintReadOnly, Category="AttributeSet", ReplicatedUsing = OnRep_MaxStamina, DisplayName="最大体力")
	FGameplayAttributeData MaxStamina;
	ATTRIBUTE_ACCESSORS(UGAS_AttributeSetBase, MaxStamina);

	UPROPERTY(BlueprintReadOnly, Category="AttributeSet", ReplicatedUsing = OnRep_MaxMovementSpeed, DisplayName="最大移动速度")
	FGameplayAttributeData MaxMovementSpeed;
	ATTRIBUTE_ACCESSORS(UGAS_AttributeSetBase, MaxMovementSpeed);

protected:
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

	UFUNCTION()
	virtual void OnRep_Health(const FGameplayAttributeData& OldHealth);

	UFUNCTION()
	virtual void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth);

	UFUNCTION()
	virtual void OnRep_Stamina(const FGameplayAttributeData& OldStamina);

	UFUNCTION()
	virtual void OnRep_MaxStamina(const FGameplayAttributeData& OldMaxStamina);
	
	UFUNCTION()
	virtual void OnRep_MaxMovementSpeed(const FGameplayAttributeData& OldMaxMovementSpeed);

	// 复制
	virtual void GetLifetimeReplicatedProps( TArray< class FLifetimeProperty > & OutLifetimeProps ) const override;
};
