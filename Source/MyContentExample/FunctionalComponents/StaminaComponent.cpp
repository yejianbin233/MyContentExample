// Fill out your copyright notice in the Description page of Project Settings.


#include "FunctionalComponents/StaminaComponent.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

static TAutoConsoleVariable<int32> CVarDebugStaminaComponent(
TEXT("ShowStaminaComponentDebugInfo"),
	0,
	TEXT("显示或隐藏体力组件的调试信息。")
	TEXT("  0: off")
	TEXT("  1: on"),
	ECVF_Scalability | ECVF_RenderThreadSafe);

// Sets default values for this component's properties
UStaminaComponent::UStaminaComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UStaminaComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	GetWorld()->GetTimerManager().SetTimer(ReplyStaminaTimerHandle, this, &UStaminaComponent::ReplyStamina, ReplyRate, true);
}

bool UStaminaComponent::CanRun()
{
	float MidValue = (MaxStamina + MinStamina) / 2;
	if (Stamina >= MidValue)
	{
		return true;
	}

	return false;
}

void UStaminaComponent::ReplyStamina()
{
	float calStamina;

	const float CurrentStamina = Stamina;
	
	ACharacter* Character = Cast<ACharacter>(GetOwner());
	UCharacterMovementComponent* MovementComponent = Character->GetCharacterMovement();
	if (MovementComponent->Velocity.Length() == 0)
	{
		// 判断角色是否移动，如果不移动则设置为空闲状态。
		StaminaState = EStaminaState::Idle;
		OnStaminaStateChanged.Broadcast(StaminaState);
	}

	FString StateString;
	switch (StaminaState)
	{
		case EStaminaState::Idle :
			calStamina = IdleReplySpeed;
			StateString = "Idle";
			break;
		case EStaminaState::Walk :
			calStamina = WalkReplySpeed + WalkConsumeSpeed;
			StateString = "Walk";
			break;
		case EStaminaState::Run :
			calStamina = RunConsumeSpeed + RunReplySpeed;
			StateString = "Run";
			break;
		default:
			calStamina = 0;
			break;
	}

	
	Stamina = FMath::Clamp( CurrentStamina + calStamina, MinStamina, MaxStamina);

	float IsDebug = CVarDebugStaminaComponent.GetValueOnGameThread();
	if (IsDebug)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Green, FString::Printf(TEXT("当前状态：{%s}, 当前状态计算前值：{%f}, 差值: {%f}, 结果值：{%f}"), *StateString, CurrentStamina, calStamina, Stamina));
	}
	// 广播体力值改变
	OnStaminaChanged.Broadcast(Stamina);
	if (StaminaState == EStaminaState::Run && !CanRun())
	{
		StaminaState = EStaminaState::Walk;
		OnStaminaStateChanged.Broadcast(StaminaState);
	}
}

void UStaminaComponent::ChangedStaminaState(EStaminaState InStaminaState)
{
	StaminaState = InStaminaState;
	OnStaminaStateChanged.Broadcast(StaminaState);
}

#if WITH_EDITOR
void UStaminaComponent::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	FName PropertyName = PropertyChangedEvent.Property != nullptr ? PropertyChangedEvent.Property->GetFName() : NAME_None;

	if (PropertyName == GET_MEMBER_NAME_CHECKED(UStaminaComponent, MaxStamina))
	{
		MaxStamina = FMath::Max(MaxStamina, MinStamina);
	}

	if (PropertyName == GET_MEMBER_NAME_CHECKED(UStaminaComponent, MinStamina))
	{
		MinStamina = FMath::Min(MaxStamina, MinStamina);
	}

	if (PropertyName == GET_MEMBER_NAME_CHECKED(UStaminaComponent, Stamina))
	{
		Stamina = FMath::Clamp(Stamina, MinStamina, MaxStamina);
	}
}

#endif
