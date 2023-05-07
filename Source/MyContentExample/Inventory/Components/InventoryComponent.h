// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Inventory/InventoryList.h"
#include "GameplayTagContainer.h"
#include "InventoryComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInventoryItemDataChanged, TArray<UInventoryItemInstance*>, ChangedInventoryItemInstanceList);

UCLASS(BlueprintType, Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MYCONTENTEXAMPLE_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	static FGameplayTag EquipItemActorTag;
	static FGameplayTag DropItemTag;
	static FGameplayTag EquipNextTag;
	static FGameplayTag UnequipTag;
	static FGameplayTag PickItemActorTag;
	static FGameplayTag DropWeaponTag;
	static FGameplayTag OpenOrCloseInventoryWidgetTag;

	UPROPERTY(BlueprintAssignable, Category="Events")
	FOnInventoryItemDataChanged InventoryItemDataChanged;

protected:

	UPROPERTY(Replicated)
	FInventoryList InventoryList;

	// TODO DELETE
	UPROPERTY(EditDefaultsOnly)
	TArray<TSubclassOf<UItemStaticData>> DefaultItems;

	// TODO DELETE
	UPROPERTY(Replicated)
	UInventoryItemInstance* CurrentItem = nullptr;
	

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UMultiInventoryMainWidget> InventoryWidgetClass;

	UPROPERTY(BlueprintReadOnly)
	UMultiInventoryMainWidget* InventoryMainWidget;

public:	
	// Sets default values for this component's properties
	UInventoryComponent();

	virtual void InitializeComponent() override;

	virtual bool ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags) override;

	UFUNCTION(BlueprintCallable)
	void AddItem(TSubclassOf<UItemStaticData> InItemStaticDataClass);

	UFUNCTION(BlueprintCallable)
	void AddItemInstance(UInventoryItemInstance* InItemInstance);
	
	UFUNCTION(BlueprintCallable)
	void RemoveItem(TSubclassOf<UItemStaticData> InItemStaticDataClass);

	// UFUNCTION(BlueprintCallable)
	// void EquipItem(TSubclassOf<UItemStaticData> InItemStaticDataClass);
	//
	// UFUNCTION(BlueprintCallable)
	// void UnEquipItem(TSubclassOf<UItemStaticData> InItemStaticDataClass);
	//
	// UFUNCTION(BlueprintCallable)
	// void DropItem();

	virtual void GameplayEventCallback(const FGameplayEventData* Payload);

	UFUNCTION()
	void AddGameplayTags();

protected:

	// 通过游戏标签和游戏事件来处理仓库。
	void HandleGameplayEventInternal(FGameplayEventData Payload);

	// 服务器处理仓库的游戏事件
	UFUNCTION(Server, Reliable)
	void ServerHandleGameplayEvent(FGameplayEventData Payload);
	
	// Called when the game starts
	virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// 在蓝图中实现每个子类不同的拾取物品逻辑
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category="Inventory | Pick")
	void Pick();

	UFUNCTION(BlueprintCallable)
	void OpenOrCloseInventoryWidget();

	void OpenInventoryWidget();

	void CloseInventoryWidget();

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	FORCEINLINE UInventoryItemInstance* GetCurrentItem() const { return CurrentItem; };

	const FInventoryList& GetInventoryList() const { return InventoryList; };

	TArray<UInventoryItemInstance*> GetInventoryItemInstances();
};
