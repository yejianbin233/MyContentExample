// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemActor.generated.h"

class UItemStaticData;
class UInventoryItemInstance;

UCLASS()
class MYCONTENTEXAMPLE_API AItemActor : public AActor
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UItemStaticData> ItemStaticDataClass;
	
	UPROPERTY(ReplicatedUsing = OnRep_ItemInstance)
	UInventoryItemInstance* ItemInstance = nullptr;
	
public:	
	// Sets default values for this actor's properties
	AItemActor();

	virtual void OnEquipped();
	virtual void OnUnequipped();
	virtual void OnDropped();
	void Init(UInventoryItemInstance* InItemInstance);

	virtual bool ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// 拓展，用于子类的内部初始化
	virtual void InitInternal();
	
	UFUNCTION()
	void OnRep_ItemInstance(UInventoryItemInstance* InItemInstance);
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
