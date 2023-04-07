// Fill out your copyright notice in the Description page of Project Settings.


#include "Camera/CameraControllerComponent.h"

#include "GameFramework/SpringArmComponent.h"

static TAutoConsoleVariable<int32> CVarDebugCameraControllerComponent(
TEXT("ShowCameraControllerComponentDebugInfo"),
	0,
	TEXT("显示或隐藏摄像机控制组件的调试信息。")
	TEXT("  0: off")
	TEXT("  1: on"),
	ECVF_Scalability | ECVF_RenderThreadSafe);

// Sets default values for this component's properties
UCameraControllerComponent::UCameraControllerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


void UCameraControllerComponent::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	FName PropertyName = PropertyChangedEvent.Property != nullptr ? PropertyChangedEvent.Property->GetFName() : NAME_None;

	if (PropertyName == GET_MEMBER_NAME_CHECKED(UCameraControllerComponent, MaxSpringArmLength))
	{
		MaxSpringArmLength = FMath::Max(MaxSpringArmLength, MinSpringArmLength);
	}

	if (PropertyName == GET_MEMBER_NAME_CHECKED(UCameraControllerComponent, MinSpringArmLength))
	{
		MinSpringArmLength = FMath::Min(MaxSpringArmLength, MinSpringArmLength);
	}
}

// Called when the game starts
void UCameraControllerComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UCameraControllerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UCameraControllerComponent::InitReferences(USpringArmComponent* InCameraBoom, UCameraComponent* InFollowCamera)
{
	CameraBoom = InCameraBoom;
	FollowCamera = InFollowCamera;
}

void UCameraControllerComponent::Zoom(float ZoomDirectional) const
{
	if (CameraBoom)
	{
		const float ZoomValue = ZoomDirectional * ZoomSpeed;

		const float CurrentArmLength = CameraBoom->TargetArmLength;
		const float CalTargetArmLength = CurrentArmLength + ZoomValue;
		const float FinalTargetArmLength = FMath::Clamp(CalTargetArmLength, MinSpringArmLength, MaxSpringArmLength);
		CameraBoom->TargetArmLength = FinalTargetArmLength;

		int32 IsDebug = CVarDebugCameraControllerComponent.GetValueOnGameThread();
		if (IsDebug)
		{
			GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Green, FString::Printf(TEXT("缩放前弹簧臂长度：{%f}, 缩放后弹簧臂长度: {%f}"), CurrentArmLength, FinalTargetArmLength));
		}
	}
}

