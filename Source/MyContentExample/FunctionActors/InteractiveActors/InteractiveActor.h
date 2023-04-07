// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractiveActor.generated.h"

UCLASS()
class MYCONTENTEXAMPLE_API AInteractiveActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInteractiveActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/*
	 * 冗余设计，每个可交互 Actor 按照设计都是继承 AInteractiveActor 类，但同一个可交互 Actor 可能有不同数量的交互逻辑，因此使用 int32 类型作为参数。
	 *
	 * 且在此基础上，每个继承了 AInteractiveActor 的类都需要使用自己定义的枚举类来使 InteractiveId 与之对应，而不至于使 InteractiveId 变成魔法参数（未知参数）。
	 */
	UFUNCTION(BlueprintCallable, Category="Interactive")
	virtual void Interactive(int32 InteractiveId);

};
