// Fill out your copyright notice in the Description page of Project Settings.


#include "FunctionActors/MinimapActor.h"

#include "Components/SceneCaptureComponent2D.h"

// Sets default values
AMinimapActor::AMinimapActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MinimapSceneCapture2DComponent = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("MinimapSceneCapture2DComponent"));

	MinimapSceneCapture2DComponent->SetupAttachment(GetDefaultAttachComponent());
}

// Called when the game starts or when spawned
void AMinimapActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMinimapActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMinimapActor::UpdateMinimap_Implementation()
{
	
}

