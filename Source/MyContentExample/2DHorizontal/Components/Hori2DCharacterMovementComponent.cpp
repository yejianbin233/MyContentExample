// Fill out your copyright notice in the Description page of Project Settings.


#include "2DHorizontal/Components/Hori2DCharacterMovementComponent.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "2DHorizontal/PaperZDHorizontalCharacter.h"
#include "2DHorizontal/GameDatas/MovementType.h"
#include "GameFramework/Character.h"

static TAutoConsoleVariable<int32> CVarDebugListenPlayerInputKeyState(
TEXT("ShowListenPlayerInputKeyStateInfo"),
	0,
	TEXT("显示或隐藏监听玩家按键输入的调试信息。")
	TEXT("  0: off")
	TEXT("  1: on"),
	ECVF_Scalability | ECVF_RenderThreadSafe);


void UHori2DCharacterMovementComponent::InitPlayerInputKeyListen(APaperZDHorizontalCharacter* PlayerCharacter)
{
	if (APlayerController* PC = Cast<APlayerController>(PlayerCharacter->GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	UEnhancedInputComponent* PlayerInputComponent = Cast<UEnhancedInputComponent>(PlayerCharacter->InputComponent);
	if (PlayerInputComponent)
	{
		//Jumping
		PlayerInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, PlayerCharacter, &APaperZDHorizontalCharacter::ExecMoveAction);

		//Moving
		PlayerInputComponent->BindAction(JumpAction, ETriggerEvent::Started, PlayerCharacter, &APaperZDHorizontalCharacter::ExecJumpAction);
		
		PlayerInputComponent->BindAction(W, ETriggerEvent::Started, this, &UHori2DCharacterMovementComponent::OnW_Pressed);
		PlayerInputComponent->BindAction(W, ETriggerEvent::Triggered, this, &UHori2DCharacterMovementComponent::OnW_Hold);
		PlayerInputComponent->BindAction(W, ETriggerEvent::Completed, this, &UHori2DCharacterMovementComponent::OnW_Relax);
		PlayerInputComponent->BindAction(W, ETriggerEvent::Canceled, this, &UHori2DCharacterMovementComponent::OnW_Relax);

		PlayerInputComponent->BindAction(S, ETriggerEvent::Started, this, &UHori2DCharacterMovementComponent::OnS_Pressed);
		PlayerInputComponent->BindAction(S, ETriggerEvent::Triggered, this, &UHori2DCharacterMovementComponent::OnS_Hold);
		PlayerInputComponent->BindAction(S, ETriggerEvent::Completed, this, &UHori2DCharacterMovementComponent::OnS_Relax);
		PlayerInputComponent->BindAction(S, ETriggerEvent::Canceled, this, &UHori2DCharacterMovementComponent::OnS_Relax);
		
		PlayerInputComponent->BindAction(A, ETriggerEvent::Started, this, &UHori2DCharacterMovementComponent::OnA_Pressed);
		PlayerInputComponent->BindAction(A, ETriggerEvent::Triggered, this, &UHori2DCharacterMovementComponent::OnA_Hold);
		PlayerInputComponent->BindAction(A, ETriggerEvent::Completed, this, &UHori2DCharacterMovementComponent::OnA_Relax);
		PlayerInputComponent->BindAction(A, ETriggerEvent::Canceled, this, &UHori2DCharacterMovementComponent::OnA_Relax);
		
		PlayerInputComponent->BindAction(D, ETriggerEvent::Started, this, &UHori2DCharacterMovementComponent::OnD_Pressed);
		PlayerInputComponent->BindAction(D, ETriggerEvent::Triggered, this, &UHori2DCharacterMovementComponent::OnD_Hold);
		PlayerInputComponent->BindAction(D, ETriggerEvent::Completed, this, &UHori2DCharacterMovementComponent::OnD_Relax);
		PlayerInputComponent->BindAction(D, ETriggerEvent::Canceled, this, &UHori2DCharacterMovementComponent::OnD_Relax);
		
		PlayerInputComponent->BindAction(J, ETriggerEvent::Started, this, &UHori2DCharacterMovementComponent::OnJ_Pressed);
		PlayerInputComponent->BindAction(J, ETriggerEvent::Triggered, this, &UHori2DCharacterMovementComponent::OnJ_Hold);
		PlayerInputComponent->BindAction(J, ETriggerEvent::Completed, this, &UHori2DCharacterMovementComponent::OnJ_Relax);
		PlayerInputComponent->BindAction(J, ETriggerEvent::Canceled, this, &UHori2DCharacterMovementComponent::OnJ_Relax);
		
		PlayerInputComponent->BindAction(K, ETriggerEvent::Started, this, &UHori2DCharacterMovementComponent::OnK_Pressed);
		PlayerInputComponent->BindAction(K, ETriggerEvent::Triggered, this, &UHori2DCharacterMovementComponent::OnK_Hold);
		PlayerInputComponent->BindAction(K, ETriggerEvent::Completed, this, &UHori2DCharacterMovementComponent::OnK_Relax);
		PlayerInputComponent->BindAction(K, ETriggerEvent::Canceled, this, &UHori2DCharacterMovementComponent::OnK_Relax);
		
		PlayerInputComponent->BindAction(L, ETriggerEvent::Started, this, &UHori2DCharacterMovementComponent::OnL_Pressed);
		PlayerInputComponent->BindAction(L, ETriggerEvent::Triggered, this, &UHori2DCharacterMovementComponent::OnL_Hold);
		PlayerInputComponent->BindAction(L, ETriggerEvent::Completed, this, &UHori2DCharacterMovementComponent::OnL_Relax);
		PlayerInputComponent->BindAction(L, ETriggerEvent::Canceled, this, &UHori2DCharacterMovementComponent::OnL_Relax);
	}
}

void UHori2DCharacterMovementComponent::OnW_Pressed()
{
	W_KeyState = EPlayerInputKeyState::Pressed;

	int32 ShowListenPlayerInputKeyInfo = CVarDebugListenPlayerInputKeyState.GetValueOnAnyThread();
	if (ShowListenPlayerInputKeyInfo)
	{
		ShowDebugInfo("W", W_KeyState);
	}
}

void UHori2DCharacterMovementComponent::OnW_Relax()
{
	W_KeyState = EPlayerInputKeyState::Relax;

	int32 ShowListenPlayerInputKeyInfo = CVarDebugListenPlayerInputKeyState.GetValueOnAnyThread();
	if (ShowListenPlayerInputKeyInfo)
	{
		ShowDebugInfo("W", W_KeyState);
	}
}

void UHori2DCharacterMovementComponent::OnW_Hold()
{
	W_KeyState = EPlayerInputKeyState::Hold;

	int32 ShowListenPlayerInputKeyInfo = CVarDebugListenPlayerInputKeyState.GetValueOnAnyThread();
	if (ShowListenPlayerInputKeyInfo)
	{
		ShowDebugInfo("W", W_KeyState);
	}
}

void UHori2DCharacterMovementComponent::OnS_Pressed()
{
	S_KeyState = EPlayerInputKeyState::Pressed;

	int32 ShowListenPlayerInputKeyInfo = CVarDebugListenPlayerInputKeyState.GetValueOnAnyThread();
	if (ShowListenPlayerInputKeyInfo)
	{
		ShowDebugInfo("S", S_KeyState);
	}
}

void UHori2DCharacterMovementComponent::OnS_Relax()
{
	S_KeyState = EPlayerInputKeyState::Relax;

	int32 ShowListenPlayerInputKeyInfo = CVarDebugListenPlayerInputKeyState.GetValueOnAnyThread();
	if (ShowListenPlayerInputKeyInfo)
	{
		ShowDebugInfo("S", S_KeyState);
	}
}

void UHori2DCharacterMovementComponent::OnS_Hold()
{
	S_KeyState = EPlayerInputKeyState::Hold;

	int32 ShowListenPlayerInputKeyInfo = CVarDebugListenPlayerInputKeyState.GetValueOnAnyThread();
	if (ShowListenPlayerInputKeyInfo)
	{
		ShowDebugInfo("S", S_KeyState);
	}
}

void UHori2DCharacterMovementComponent::OnA_Pressed()
{
	A_KeyState = EPlayerInputKeyState::Pressed;

	int32 ShowListenPlayerInputKeyInfo = CVarDebugListenPlayerInputKeyState.GetValueOnAnyThread();
	if (ShowListenPlayerInputKeyInfo)
	{
		ShowDebugInfo("A", A_KeyState);
	}
}

void UHori2DCharacterMovementComponent::OnA_Relax()
{
	A_KeyState = EPlayerInputKeyState::Relax;

	int32 ShowListenPlayerInputKeyInfo = CVarDebugListenPlayerInputKeyState.GetValueOnAnyThread();
	if (ShowListenPlayerInputKeyInfo)
	{
		ShowDebugInfo("A", A_KeyState);
	}
}

void UHori2DCharacterMovementComponent::OnA_Hold()
{
	A_KeyState = EPlayerInputKeyState::Hold;

	int32 ShowListenPlayerInputKeyInfo = CVarDebugListenPlayerInputKeyState.GetValueOnAnyThread();
	if (ShowListenPlayerInputKeyInfo)
	{
		ShowDebugInfo("A", A_KeyState);
	}
}

void UHori2DCharacterMovementComponent::OnD_Pressed()
{
	D_KeyState = EPlayerInputKeyState::Pressed;

	int32 ShowListenPlayerInputKeyInfo = CVarDebugListenPlayerInputKeyState.GetValueOnAnyThread();
	if (ShowListenPlayerInputKeyInfo)
	{
		ShowDebugInfo("D", D_KeyState);
	}
}

void UHori2DCharacterMovementComponent::OnD_Relax()
{
	D_KeyState = EPlayerInputKeyState::Relax;

	int32 ShowListenPlayerInputKeyInfo = CVarDebugListenPlayerInputKeyState.GetValueOnAnyThread();
	if (ShowListenPlayerInputKeyInfo)
	{
		ShowDebugInfo("D", D_KeyState);
	}
}

void UHori2DCharacterMovementComponent::OnD_Hold()
{
	D_KeyState = EPlayerInputKeyState::Hold;

	int32 ShowListenPlayerInputKeyInfo = CVarDebugListenPlayerInputKeyState.GetValueOnAnyThread();
	if (ShowListenPlayerInputKeyInfo)
	{
		ShowDebugInfo("D", D_KeyState);
	}
}

void UHori2DCharacterMovementComponent::OnJ_Pressed()
{
	J_KeyState = EPlayerInputKeyState::Pressed;

	int32 ShowListenPlayerInputKeyInfo = CVarDebugListenPlayerInputKeyState.GetValueOnAnyThread();
	if (ShowListenPlayerInputKeyInfo)
	{
		ShowDebugInfo("J", J_KeyState);
	}
}

void UHori2DCharacterMovementComponent::OnJ_Relax()
{
	J_KeyState = EPlayerInputKeyState::Relax;
	
	int32 ShowListenPlayerInputKeyInfo = CVarDebugListenPlayerInputKeyState.GetValueOnAnyThread();
	if (ShowListenPlayerInputKeyInfo)
	{
		ShowDebugInfo("J", J_KeyState);
	}
}

void UHori2DCharacterMovementComponent::OnJ_Hold()
{
	J_KeyState = EPlayerInputKeyState::Hold;

	int32 ShowListenPlayerInputKeyInfo = CVarDebugListenPlayerInputKeyState.GetValueOnAnyThread();
	if (ShowListenPlayerInputKeyInfo)
	{
		ShowDebugInfo("J", J_KeyState);
	}
}

void UHori2DCharacterMovementComponent::OnK_Pressed()
{
	K_KeyState = EPlayerInputKeyState::Pressed;

	int32 ShowListenPlayerInputKeyInfo = CVarDebugListenPlayerInputKeyState.GetValueOnAnyThread();
	if (ShowListenPlayerInputKeyInfo)
	{
		ShowDebugInfo("K", K_KeyState);
	}
}

void UHori2DCharacterMovementComponent::OnK_Relax()
{
	K_KeyState = EPlayerInputKeyState::Relax;

	int32 ShowListenPlayerInputKeyInfo = CVarDebugListenPlayerInputKeyState.GetValueOnAnyThread();
	if (ShowListenPlayerInputKeyInfo)
	{
		ShowDebugInfo("K", K_KeyState);
	}
}

void UHori2DCharacterMovementComponent::OnK_Hold()
{
	K_KeyState = EPlayerInputKeyState::Hold;

	int32 ShowListenPlayerInputKeyInfo = CVarDebugListenPlayerInputKeyState.GetValueOnAnyThread();
	if (ShowListenPlayerInputKeyInfo)
	{
		ShowDebugInfo("K", K_KeyState);
	}
}

void UHori2DCharacterMovementComponent::OnL_Pressed()
{
	L_KeyState = EPlayerInputKeyState::Pressed;

	int32 ShowListenPlayerInputKeyInfo = CVarDebugListenPlayerInputKeyState.GetValueOnAnyThread();
	if (ShowListenPlayerInputKeyInfo)
	{
		ShowDebugInfo("L", L_KeyState);
	}
}

void UHori2DCharacterMovementComponent::OnL_Relax()
{
	L_KeyState = EPlayerInputKeyState::Relax;

	int32 ShowListenPlayerInputKeyInfo = CVarDebugListenPlayerInputKeyState.GetValueOnAnyThread();
	if (ShowListenPlayerInputKeyInfo)
	{
		ShowDebugInfo("L", L_KeyState);
	}
}

void UHori2DCharacterMovementComponent::OnL_Hold()
{
	L_KeyState = EPlayerInputKeyState::Hold;

	int32 ShowListenPlayerInputKeyInfo = CVarDebugListenPlayerInputKeyState.GetValueOnAnyThread();
	if (ShowListenPlayerInputKeyInfo)
	{
		ShowDebugInfo("L", L_KeyState);
	}
}

void UHori2DCharacterMovementComponent::ShowDebugInfo(FString KeyName, EPlayerInputKeyState KeyState)
{
	FString KeyStateString;

	switch (KeyState)
	{
		case EPlayerInputKeyState::Hold:
			KeyStateString = "On Holding State";
			break;
		case EPlayerInputKeyState::Pressed:
			KeyStateString = "On Pressed State";
			break;
		case EPlayerInputKeyState::Relax:
			KeyStateString = "On Relax State";
			break;
		default: ;
	}
	GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Green, FString::Printf(TEXT("按键(%s) %s"), *KeyName, *KeyStateString));
}
