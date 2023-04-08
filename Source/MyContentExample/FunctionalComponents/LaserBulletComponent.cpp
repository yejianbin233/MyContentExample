// Fill out your copyright notice in the Description page of Project Settings.


#include "FunctionalComponents/LaserBulletComponent.h"

#include "NiagaraComponent.h"

// Sets default values for this component's properties
ULaserBulletComponent::ULaserBulletComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	LaserNiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("LaserNiagaraComponent"));
	// ...
}

void ULaserBulletComponent::Interactive_Implementation(ELaserState LaserState)
{
	
}


void ULaserBulletComponent::UpdateEndPosition_Implementation()
{
	
}

// Called when the game starts
void ULaserBulletComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


