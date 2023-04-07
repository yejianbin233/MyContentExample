// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MinimapActor.generated.h"

UCLASS()
class MYCONTENTEXAMPLE_API AMinimapActor : public AActor
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Minimap", meta = (AllowPrivateAccess = "true"))
	class USceneCaptureComponent2D* MinimapSceneCapture2DComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Minimap | Widget", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UUserWidget> MinimapWidgetClass;
	
	UPROPERTY(BlueprintReadWrite, Category="Minimap | Widget", meta = (AllowPrivateAccess = "true"))
	class UUserWidget* MinimapWidget;
	
public:	
	// Sets default values for this actor's properties
	AMinimapActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
