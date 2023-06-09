// Copyright Epic Games, Inc. All Rights Reserved.

#include "GAS_Character.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameplayEffectTypes.h"
#include "NiagaraComponent.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Camera/CameraControllerComponent.h"
#include "FunctionActors/MinimapActor.h"
#include "FunctionalComponents/GrabComponent.h"
#include "FunctionalComponents/LaserBulletComponent.h"
#include "FunctionalComponents/StaminaComponent.h"
#include "GAS/Components/GAS_AbilitySystemComponentBase.h"
#include "GAS/AttributeSets/GAS_AttributeSetBase.h"
#include "GAS/Components/GAS_CharacterMovementComponent.h"
#include "GAS/Components/GAS_FootstepsComponent.h"
#include "GAS/Components/GAS_MotionWarpingComponent.h"
#include "GAS/DataAssets/GAS_CharacterDataAsset.h"
#include "Inventory/Components/InventoryComponent.h"
#include "Net/UnrealNetwork.h"
//////////////////////////////////////////////////////////////////////////
// AGAS_Character

AGAS_Character::AGAS_Character(const FObjectInitializer& ObjectInitializer):
	Super(ObjectInitializer.SetDefaultSubobjectClass<UGAS_CharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = DefaultMaxMovementSpeed;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)

	/*============ GAS ============*/
	AbilitySystemComponent = CreateDefaultSubobject<UGAS_AbilitySystemComponentBase>(TEXT("AbilitySystemComponent"));
	//  GAS 组件复制功能相关设置
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
	
	AttributeSet = CreateDefaultSubobject<UGAS_AttributeSetBase>(TEXT("AttributeSet"));

	/*============ GAS ============*/
}

void AGAS_Character::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if(IsValid(CharacterDataAsset))
	{
		SetGASCharacterData(CharacterDataAsset->GASCharacterData);
	}
}

bool AGAS_Character::ApplyGameplayEffecToSelf(TSubclassOf<UGameplayEffect> Effect,
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

void AGAS_Character::SetGASCharacterData(const FGASCharacterData& InCharacterData)
{
	CharacterData = InCharacterData;
	InitFromCharacterData(CharacterData);
}

void AGAS_Character::PawnClientRestart()
{
	Super::PawnClientRestart();

	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
		{
			Subsystem->ClearAllMappings();
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void AGAS_Character::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void AGAS_Character::StartJump(const FInputActionValue& Value)
{
	GetGasCharacterMovementComponent()->TryTraversal(GetAbilitySystemComponent());
}

void AGAS_Character::StopJumping()
{
	// Super::StopJumping();
}

void AGAS_Character::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);
	
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->RemoveActiveEffectsWithTags(InAirTags);

		FGameplayTagContainer GameplayTagContainer = FGameplayTagContainer::EmptyContainer;
		GameplayTagContainer.AddTag(GA_JumpTag);

		AbilitySystemComponent->CancelAbilities(&GameplayTagContainer);
	}
}

void AGAS_Character::Pick()
{
	FGameplayEventData Payload;
	Payload.EventTag = UInventoryComponent::PickItemActorTag;
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(this, UInventoryComponent::PickItemActorTag, Payload);
}

void AGAS_Character::Drop()
{
	// 只能丢弃当前武器 - TODO
	
}

void AGAS_Character::OpenOrCloseInventoryWidget()
{
	// 打开或关闭仓库用户控件
	FGameplayEventData Payload;
	Payload.EventTag = UInventoryComponent::OpenOrCloseInventoryWidgetTag;
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(this, UInventoryComponent::OpenOrCloseInventoryWidgetTag, Payload);
}

void AGAS_Character::OnRep_CharacterData(FGASCharacterData InCharacterData)
{
	InitFromCharacterData(InCharacterData, true);
}

UAbilitySystemComponent* AGAS_Character::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

//////////////////////////////////////////////////////////////////////////
// Input

void AGAS_Character::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		//Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &AGAS_Character::StartJump);
		
		//Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AGAS_Character::Move);

		//Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AGAS_Character::Look);
		
		EnhancedInputComponent->BindAction(PickAction, ETriggerEvent::Started, this, &AGAS_Character::Pick);
		EnhancedInputComponent->BindAction(DropAction, ETriggerEvent::Started, this, &AGAS_Character::Drop);
		EnhancedInputComponent->BindAction(OpenOrCloseInventoryWidgetAction, ETriggerEvent::Started, this, &AGAS_Character::OpenOrCloseInventoryWidget);
	}
}

void AGAS_Character::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AGAS_Character::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AGAS_Character::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AGAS_Character, CharacterData);
}

void AGAS_Character::GiveAbilities()
{
	if(HasAuthority() && AbilitySystemComponent)
	{
		// 遍历技能
		for (auto DefaultAbility : CharacterData.Abilities)
		{
			// 授予技能
			AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(DefaultAbility));
		}
	}
}

void AGAS_Character::ApplyStartupGameplayEffects()
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

void AGAS_Character::InitFromCharacterData(const FGASCharacterData& InCharacterData, bool bFromReplication)
{
	
}

void AGAS_Character::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// 服务器设置 GAS 组件初始化 Actor Info
	AbilitySystemComponent->InitAbilityActorInfo(this, this);

	// 授予技能
	GiveAbilities();

	// 应用初始 GameplayEffect
	ApplyStartupGameplayEffects();
}

void AGAS_Character::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	AbilitySystemComponent->InitAbilityActorInfo(this, this);
}




