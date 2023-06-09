// Fill out your copyright notice in the Description page of Project Settings.


#include "2DHorizontal/PaperZDHorizontalCharacter.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "Components/Hori2DCharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetMathLibrary.h"

// extern TAutoConsoleVariable<int32> CVarDebugListenPlayerInputKeyState;

APaperZDHorizontalCharacter::APaperZDHorizontalCharacter(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer.SetDefaultSubobjectClass<UHori2DCharacterMovementComponent>(CharacterMovementComponentName))
{
	CameraSprintArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraSprintArmComponent"));
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));

	CameraSprintArmComponent->SetupAttachment(GetRootComponent());
	CameraComponent->AttachToComponent(CameraSprintArmComponent,  FAttachmentTransformRules::KeepRelativeTransform, USpringArmComponent::SocketName);

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = false;
}

void APaperZDHorizontalCharacter::ExecMoveAction(const FInputActionValue& Value)
{
	// 暂时不使用该处来处理移动输入
	FVector ActorForwardVector = GetActorForwardVector();
	
	FVector2D MovementValue = Value.Get<FVector2D>();
	
	if (Controller)
	{
		AddMovementInput(ActorForwardVector, MovementValue.X);
	}

	UHori2DCharacterMovementComponent* Hori2DCharacterMovementComponent = Cast<UHori2DCharacterMovementComponent>(GetMovementComponent());
	if (Hori2DCharacterMovementComponent)
	{
		Hori2DCharacterMovementComponent->InputHandle(EMovementStateType::OnMove);
	}
}

void APaperZDHorizontalCharacter::ExecJumpAction(const FInputActionValue& Value)
{
	UHori2DCharacterMovementComponent* Hori2DCharacterMovementComponent = Cast<UHori2DCharacterMovementComponent>(GetMovementComponent());
	if (Hori2DCharacterMovementComponent)
	{
		Hori2DCharacterMovementComponent->InputHandle(EMovementStateType::OnJumping);
	}
	// Jump();
}

void APaperZDHorizontalCharacter::Landed(const FHitResult& Hit)
{
	UHori2DCharacterMovementComponent* Hori2DCharacterMovementComponent = Cast<UHori2DCharacterMovementComponent>(GetMovementComponent());
	if (Hori2DCharacterMovementComponent)
	{
		Hori2DCharacterMovementComponent->InputHandle(EMovementStateType::OnLanded);
	}

	Super::Landed(Hit);
}

void APaperZDHorizontalCharacter::Falling()
{
	UCharacterMovementComponent* MovementComponent = GetCharacterMovement();

	if (MovementComponent)
	{
		const FVector CharacterVelocityDirectional = UKismetMathLibrary::Normal(MovementComponent->Velocity);
		const FVector CharacterUpDirectional = GetActorUpVector();

		int value = UKismetMathLibrary::Dot_VectorVector(CharacterVelocityDirectional, CharacterUpDirectional);
		bool IsFalling = UKismetMathLibrary::Dot_VectorVector(CharacterVelocityDirectional, CharacterUpDirectional) <= 0;

		// int32 ShowListenPlayerInputKeyInfo = CVarDebugListenPlayerInputKeyState.GetValueOnAnyThread();
		// if (ShowListenPlayerInputKeyInfo)
		// {
		// 	GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Green,
		// 								 FString::Printf(
		// 									 TEXT("速度方向: %s，向上方向：%s, 点乘值: %d"),
		// 									 *CharacterVelocityDirectional.ToString(),
		// 									 *CharacterUpDirectional.ToString(), value));
		// }

		UHori2DCharacterMovementComponent* Hori2DCharacterMovementComponent = Cast<UHori2DCharacterMovementComponent>(GetMovementComponent());

		if (Hori2DCharacterMovementComponent)
		{
			if (IsFalling)
			{
				Hori2DCharacterMovementComponent->InputHandle(EMovementStateType::OnFalling);
			}
			else
			{
				Hori2DCharacterMovementComponent->InputHandle(EMovementStateType::OnJumping);
			}
		}
	}
	Super::Falling();
}

void APaperZDHorizontalCharacter::GiveAbilities()
{
	if(AbilitySystemComponent)
	{
		// 遍历技能
		for (const auto DefaultAbility : CharacterData.Abilities)
		{
			// 授予技能
			AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(DefaultAbility));
		}
	}
}

void APaperZDHorizontalCharacter::ApplyStartupGameplayEffects()
{
	if(GetLocalRole() == ROLE_Authority)
	{
		// 必须只有在服务器上才执行以下逻辑
		FGameplayEffectContextHandle EffectContextHandle = AbilitySystemComponent->MakeEffectContext();
		EffectContextHandle.AddSourceObject(this);

		for (auto DefaultEffect : CharacterData.Effects)
		{
			ApplyGameplayEffecToSelf(DefaultEffect, EffectContextHandle);
		}
	}
}

bool APaperZDHorizontalCharacter::ApplyGameplayEffecToSelf(TSubclassOf<UGameplayEffect> Effect,
											  FGameplayEffectContextHandle InEffectContextHandle)
{
	if (!Effect.Get()) return false;

	// 应用 GameplayEffect
	FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(Effect, 1, InEffectContextHandle);
	if (SpecHandle.IsValid())
	{
		FActiveGameplayEffectHandle ActiveGameplayEffectHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
		// 返回 GameplayEffect 应用结果。
		return ActiveGameplayEffectHandle.WasSuccessfullyApplied();
	}
	
	return false;
}

void APaperZDHorizontalCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		APlayerController* PlayerController = Cast<APlayerController>(GetController());
		if (PlayerController)
		{
			this->InputComponent = PlayerInputComponent;
			UHori2DCharacterMovementComponent* Hor2DCharacterMovementComponent = Cast<UHori2DCharacterMovementComponent>(GetCharacterMovement());
			if (Hor2DCharacterMovementComponent)
			{
				Hor2DCharacterMovementComponent->InitPlayerInputKeyListen(this);
			}
		}
	}

	
}

void APaperZDHorizontalCharacter::PawnClientRestart()
{
	Super::PawnClientRestart();
}

void APaperZDHorizontalCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// 服务器设置 GAS 组件初始化 Actor Info
	AbilitySystemComponent->InitAbilityActorInfo(this, this);

	// 授予技能
	GiveAbilities();

	// 应用初始 GameplayEffect
	ApplyStartupGameplayEffects();
}