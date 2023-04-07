// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CameraControllerComponent.generated.h"


class USpringArmComponent;
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MYCONTENTEXAMPLE_API UCameraControllerComponent : public UActorComponent
{
	GENERATED_BODY()

	// Sets default values for this component's properties
	UCameraControllerComponent();

	UPROPERTY(BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera, DisplayName= "视角缩放速率",meta = (AllowPrivateAccess = "true"))
	float ZoomSpeed = 1;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera, DisplayName= "弹簧臂最大长度", meta = (AllowPrivateAccess = "true"))
	float MaxSpringArmLength = 400;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera, DisplayName= "弹簧臂最小长度", meta = (AllowPrivateAccess = "true"))
	float MinSpringArmLength = 50;

#if WITH_EDITOR

	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

#endif
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void InitReferences(USpringArmComponent* InCameraBoom, UCameraComponent* InFollowCamera);

	void Zoom(float ZoomDirectional) const;
};
