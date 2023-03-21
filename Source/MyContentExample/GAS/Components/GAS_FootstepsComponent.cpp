// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Components/GAS_FootstepsComponent.h"

#include "GAS/Character/GAS_Character.h"
#include "GAS/PhysicalMaterials/GAS_PhysicalMaterialBase.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"

static TAutoConsoleVariable<int32> CVarShowFootsteps(TEXT("ShowDebugFootsteps"), 0,
	TEXT("Draws debug info about footsteps")
	TEXT("  0: off/n")
	TEXT("  1: on/n"),
	ECVF_Cheat);

// Sets default values for this component's properties
UGAS_FootstepsComponent::UGAS_FootstepsComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UGAS_FootstepsComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UGAS_FootstepsComponent::HandleFootstep(EFoot Foot)
{
	if(AGAS_Character* Character = Cast<AGAS_Character>(GetOwner()))
	{
		if (USkeletalMeshComponent* SkeletalMeshComponent = Character->GetMesh())
		{
			FHitResult HitResult;

			const FVector SocketLocation = SkeletalMeshComponent->GetSocketLocation(Foot == EFoot::Left ? LeftFootSocketName : RightFootSocketName);

			const FVector StartTraceLocation = SocketLocation + FVector::UpVector * 20.0f;
			const FVector EndTraceLocation = StartTraceLocation + FVector::DownVector * 50.0f;

			FCollisionQueryParams QueryParams;
			QueryParams.bReturnPhysicalMaterial = true;
			QueryParams.AddIgnoredActor(Character);

			const int32 DebugShowFootsteps = CVarShowFootsteps.GetValueOnAnyThread();

			if (GetWorld()->LineTraceSingleByChannel(HitResult
				, StartTraceLocation, EndTraceLocation,
				ECollisionChannel::ECC_WorldStatic,
				QueryParams))
			{
				
				if (HitResult.bBlockingHit)
				{
					const UGAS_PhysicalMaterialBase* PhysicalMaterial = Cast<UGAS_PhysicalMaterialBase>(HitResult.PhysMaterial.Get());

					if (HitResult.PhysMaterial.Get())
					{
						if (PhysicalMaterial)
						{
							UGameplayStatics::PlaySoundAtLocation(this, PhysicalMaterial->FootstepSound, HitResult.ImpactPoint, 1.0f);
						}

						if (DebugShowFootsteps > 0)
						{
							DrawDebugString(GetWorld(), HitResult.ImpactPoint, GetNameSafe(PhysicalMaterial), nullptr, FColor::White, 4.0f);
						}
					}
					if (DebugShowFootsteps > 0)
					{
						DrawDebugSphere(GetWorld(), SocketLocation, 16, 16, FColor::Red, false,4.0f);
					}
				}
				else
				{
					if (DebugShowFootsteps > 0)
					{
						DrawDebugLine(GetWorld(), StartTraceLocation, EndTraceLocation, FColor::Red, false,4.0f, 0, 1);
					}
				}
			}
			else
			{
				if (DebugShowFootsteps > 0)
				{
					DrawDebugLine(GetWorld(), StartTraceLocation, EndTraceLocation, FColor::Red, false,4.0f, 0, 1);
					DrawDebugSphere(GetWorld(), SocketLocation, 16, 16, FColor::Red, false,4.0f);
				}
			}
		}
	}
}

