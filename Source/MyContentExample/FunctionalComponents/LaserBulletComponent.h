// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LaserBulletComponent.generated.h"

UENUM(BlueprintType)
enum ELaserState
{
	Active UMETA(DisplayName = "激活"),
	Disable UMETA(DisplayName = "关闭"),
};

class UNiagaraComponent;

UCLASS( BlueprintType, Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MYCONTENTEXAMPLE_API ULaserBulletComponent : public UActorComponent
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite,Category="FX | Laser", meta = (AllowPrivateAccess = "true"))
	UNiagaraComponent* LaserNiagaraComponent;
	
public:	
	// Sets default values for this component's properties
	ULaserBulletComponent();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="FX | Interactive")
	void Interactive(ELaserState LaserState);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="FX | Interactive")
	void UpdateEndPosition();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
		
};
