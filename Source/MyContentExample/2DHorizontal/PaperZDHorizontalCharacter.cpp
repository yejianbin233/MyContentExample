// Fill out your copyright notice in the Description page of Project Settings.


#include "2DHorizontal/PaperZDHorizontalCharacter.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "Components/Hori2DCharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"

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
	FVector ActorForwardVector = GetActorForwardVector();

	FVector2D MovementValue = Value.Get<FVector2D>();
	
	if (Controller)
	{
		AddMovementInput(ActorForwardVector, MovementValue.X);
	}
}

void APaperZDHorizontalCharacter::ExecJumpAction(const FInputActionValue& Value)
{
	
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
}
