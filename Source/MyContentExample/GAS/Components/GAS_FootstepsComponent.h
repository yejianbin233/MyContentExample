// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GAS/GameType/GAS_GameTypes.h"
#include "GAS_FootstepsComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MYCONTENTEXAMPLE_API UGAS_FootstepsComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGAS_FootstepsComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly)
	FName LeftFootSocketName = TEXT("foot_l");

	UPROPERTY(EditDefaultsOnly)
	FName RightFootSocketName = TEXT("foot_r");

public:	

	void HandleFootstep(EFoot Foot);
};
