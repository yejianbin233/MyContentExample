// Fill out your copyright notice in the Description page of Project Settings.


#include "FunctionalComponents/GrabComponent.h"

#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"

// Sets default values for this component's properties
UGrabComponent::UGrabComponent(const FObjectInitializer& ObjectInitializer)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...

	PhysicsHandleComponent = CreateDefaultSubobject<UPhysicsHandleComponent>(TEXT("PhysicsHandleComponent"));
}

// Called when the game starts
void UGrabComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UGrabComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
	
}

void UGrabComponent::Interactive_Implementation()
{
	if (IsGrabbed)
	{
		PhysicsHandleComponent->ReleaseComponent();
		IsGrabbed = false;
	}
	
	ACharacter* Character = Cast<ACharacter>(GetOwner());

	if (Character)
	{
		APlayerController* PlayerController = Cast<APlayerController>(Character->GetController());

		if (PlayerController)
		{
			// 定义视口尺寸变量
			FVector2D ViewportSize;

			if (GEngine && GEngine->GameViewport)
			{
				// 获取视口大小
				GEngine->GameViewport->GetViewportSize(ViewportSize);
			}

			// 准星在屏幕空间中的坐标，定义为中点
			FVector2D CrosshairLocation(ViewportSize.X / 2.0f, ViewportSize.Y / 2.0f);

			FVector DeprojectWorldLocation;
			FVector DeprojectWorldDirection;

			// 将视口准星的位置投影到世界空间中
			bool bScreenToWorld = UGameplayStatics::DeprojectScreenToWorld(
					UGameplayStatics::GetPlayerController(this, 0)
					, CrosshairLocation
					, DeprojectWorldLocation
					, DeprojectWorldDirection
				);

			FHitResult HitResult;
			FVector StartPosition = DeprojectWorldLocation;
			FVector EndPosition = DeprojectWorldLocation + DeprojectWorldDirection * TraceLength;
			UKismetSystemLibrary::SphereTraceSingle(GetWorld(), StartPosition, EndPosition,
				TraceSphereRadius,
				TraceChannel,
				false,
				TArray<AActor*>(),
				DrawDebugType,
				HitResult,
				true);

			if (HitResult.bBlockingHit && HitResult.GetActor()->ActorHasTag(Tag))
			{
				PhysicsHandleComponent->GrabComponentAtLocationWithRotation(HitResult.GetComponent(),
					FName() ,
					HitResult.GetComponent()->GetComponentLocation(),
					HitResult.GetComponent()->GetComponentRotation());
				IsGrabbed = true;
			}
		}
	}
}

