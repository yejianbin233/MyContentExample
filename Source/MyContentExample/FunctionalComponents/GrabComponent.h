// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GrabComponent.generated.h"


class UPhysicsHandleComponent;
UCLASS( BlueprintType, Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MYCONTENTEXAMPLE_API UGrabComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrabComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Grab")
	UPhysicsHandleComponent* PhysicsHandleComponent;

protected:

	UPROPERTY(BlueprintReadWrite, Category="Grab")
	bool IsGrabbed;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Grab | Trace")
	float TraceLength=500.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Grab | Trace")
	float TraceSphereRadius=50.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Grab | Trace")
	TEnumAsByte<EDrawDebugTrace::Type> DrawDebugType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Grab | Trace")
	TEnumAsByte<ETraceTypeQuery> TraceChannel;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Grab | Trace")
	FName Tag;
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Grab")
	void Interactive();
		
};
