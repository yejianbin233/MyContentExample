// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "PaperZDCharacter.h"
#include "GAS/Components/GAS_AbilitySystemComponentBase.h"
#include "GAS/GameType/GAS_GameTypes.h"
#include "PaperZDHorizontalCharacter.generated.h"

struct FInputActionValue;
class USpringArmComponent;
class UCameraComponent;
struct FGameplayEffectContextHandle;
class UGameplayEffect;
/**
 * 
 */
UCLASS()
class MYCONTENTEXAMPLE_API APaperZDHorizontalCharacter : public APaperZDCharacter
{
	GENERATED_BODY()

public:


protected:
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="GAS")
	UGAS_AbilitySystemComponentBase* AbilitySystemComponent;

	UPROPERTY(EditDefaultsOnly, Category="GameplayAbilitySystem")
	FGASCharacterData CharacterData;
	
protected:
	
	UPROPERTY(EditDefaultsOnly, Category="Camera")
	UCameraComponent* CameraComponent;

	UPROPERTY(EditDefaultsOnly, Category="Camera")
	USpringArmComponent* CameraSprintArmComponent;
	
public:
	APaperZDHorizontalCharacter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	void ExecMoveAction(const FInputActionValue& Value);
    
	void ExecJumpAction(const FInputActionValue& Value);

	virtual void Landed(const FHitResult& Hit) override;

	virtual void Falling() override;
	
protected:

	// 授予初始技能
	virtual void GiveAbilities();
	
	// 应用初始 GameplayEffect
	virtual void ApplyStartupGameplayEffects();

	bool ApplyGameplayEffecToSelf(TSubclassOf<UGameplayEffect> Effect,
											  FGameplayEffectContextHandle InEffectContextHandle);
	
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	virtual void PawnClientRestart() override;

	virtual void PossessedBy(AController* NewController) override;


public:

	FORCEINLINE virtual UAbilitySystemComponent* GetAbilitySystemComponent() const { return AbilitySystemComponent; };
};
