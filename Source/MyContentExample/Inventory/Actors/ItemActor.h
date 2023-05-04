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
	
	UPROPERTY(BlueprintReadWrite, ReplicatedUsing = OnRep_ItemInstance)
	UInventoryItemInstance* ItemInstance = nullptr;

	// 每个不同的 Item Actor 需要自定义自身的功能属性，这些属性作用于场景汇总，这些属性可以进行保存，作为与仓库组件相互独立的内容，如：枪可能有填充不同数量的子弹，这些子弹不与仓库组件进行交互，而仅作为场景中枪目前可进行使用的子弹。
	
public:	
	// Sets default values for this actor's properties
	AItemActor();

	virtual void OnEquipped();
	virtual void OnUnequipped();
	virtual void OnDropped();

	UFUNCTION(BlueprintCallable, Category="Inventory", DisplayName="初始化")
	void Init(UInventoryItemInstance* InItemInstance);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Inventory")
	UInventoryItemInstance* GetInventoryItemInstance();

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
