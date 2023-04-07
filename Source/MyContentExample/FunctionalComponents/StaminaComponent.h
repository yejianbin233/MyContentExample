// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StaminaComponent.generated.h"

UENUM(BlueprintType)
enum class EStaminaState : uint8
{
	Idle UMETA(DisplayName = "空闲"),
	Walk UMETA(DisplayName = "行走"),
	Run UMETA(DisplayName = "奔跑")
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStaminaChanged, float, Stamina);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStaminaStateChanged, EStaminaState, StaminaState); 



UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MYCONTENTEXAMPLE_API UStaminaComponent : public UActorComponent
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, Category="Movement | Stamina", meta=(AllowPrivateAccess))
	float Stamina;

	UPROPERTY(EditAnywhere, Category="Movement | Stamina", meta=(AllowPrivateAccess))
	float MaxStamina = 100;

	UPROPERTY(EditAnywhere, Category="Movement | Stamina", meta=(AllowPrivateAccess))
	float MinStamina = 0;

	UPROPERTY(EditAnywhere, Category="Movement | Stamina | Consume", meta=(AllowPrivateAccess))
	float WalkConsumeSpeed = -2.0f;

	UPROPERTY(EditAnywhere, Category="Movement | Stamina | Consume", meta=(AllowPrivateAccess))
	float RunConsumeSpeed = -4.0f;

	UPROPERTY(EditAnywhere, Category="Movement | Stamina | Reply", meta=(AllowPrivateAccess))
	float WalkReplySpeed = 1.0f;

	UPROPERTY(EditAnywhere, Category="Movement | Stamina | Reply", meta=(AllowPrivateAccess))
	float RunReplySpeed = 0.0f;
	
	UPROPERTY(EditAnywhere, Category="Movement | Stamina | Reply", meta=(AllowPrivateAccess))
	float IdleReplySpeed = 10.0f;

	UPROPERTY(EditAnywhere, Category="Movement | Stamina | Reply", meta=(AllowPrivateAccess))
	float ReplyRate = 1.0f;
	
	UPROPERTY(EditAnywhere, Category="Movement | Stamina | State", meta=(AllowPrivateAccess))
	TEnumAsByte<EStaminaState> StaminaState;

	FTimerHandle ReplyStaminaTimerHandle;

public:
	
	UPROPERTY(BlueprintAssignable)
	FOnStaminaChanged OnStaminaChanged;

	UPROPERTY(BlueprintAssignable)
	FOnStaminaStateChanged OnStaminaStateChanged;
	
public:	
	// Sets default values for this component's properties
	UStaminaComponent();

#if WITH_EDITOR

	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

#endif
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	bool CanRun();

	virtual void ReplyStamina();

	void ChangedStaminaState(EStaminaState InStaminaState);

	FORCEINLINE float GetStamina() const { return  Stamina; }; 
	FORCEINLINE EStaminaState GetStaminaState() const { return  StaminaState; }; 
};
