// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FunctionActors/InteractiveActors/InteractiveActor.h"
#include "InteractiveDoorActor.generated.h"


UENUM(BlueprintType)
enum EDoorInteractiveType
{
	RevolvinDoor UMETA(DisplayName = "旋转门"),
	RisingDoor UMETA(DisplayName = "上升门"),
};
/**
 * 
 */
UCLASS()
class MYCONTENTEXAMPLE_API AInteractiveDoorActor : public AInteractiveActor
{
	GENERATED_BODY()

public:

	virtual void Interactive(int32 InteractiveId) override;

protected:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Door")
	void OpenRevolvinDoor();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Door")
	void OpenRisingDoor();
};
