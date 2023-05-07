// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/Components/InventoryComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "GameplayTagContainer.h"
#include "GameplayTagsManager.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "Engine/ActorChannel.h"
#include "GameFramework/Character.h"
#include "Inventory/InventoryItemInstance.h"
#include "Inventory/Widgets/MultiInventoryMainWidget.h"
#include "Net/UnrealNetwork.h"

extern TAutoConsoleVariable<int32> CVarDebugInventoryComponent(TEXT("DebugInventoryComponent"),
	0,
	TEXT("显示或隐藏仓库组件的调试信息。")
	TEXT("0: off/n")
	TEXT("1: on/n"),
	ECVF_Cheat);

FGameplayTag UInventoryComponent::EquipItemActorTag;
FGameplayTag UInventoryComponent::EquipNextTag;

FGameplayTag UInventoryComponent::UnequipTag;

FGameplayTag UInventoryComponent::DropItemTag;
FGameplayTag UInventoryComponent::DropWeaponTag;

FGameplayTag UInventoryComponent::PickItemActorTag;

FGameplayTag UInventoryComponent::OpenOrCloseInventoryWidgetTag;

void UInventoryComponent::HandleGameplayEventInternal(FGameplayEventData Payload)
{
	ENetRole NetRole = GetOwnerRole();

	FGameplayTag EventTag = Payload.EventTag;
	if (NetRole == ROLE_Authority)
	{

		if (EventTag == UInventoryComponent::EquipItemActorTag)
		{
			if (const UInventoryItemInstance* ItemInstance = Cast<UInventoryItemInstance>(Payload.OptionalObject))
			{
				AddItemInstance(const_cast<UInventoryItemInstance*>(ItemInstance));

				if (Payload.Instigator)
				{
					const_cast<AActor*>(Payload.Instigator.Get())->Destroy();
				}
			}
		}
		// TODO
		else if (EventTag == UInventoryComponent::PickItemActorTag)
		{
			// ...
			Pick();
		}
		
	}
	
}

void UInventoryComponent::ServerHandleGameplayEvent_Implementation(FGameplayEventData Payload)
{
	HandleGameplayEventInternal(Payload);
}

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	SetIsReplicatedByDefault(true);
	// ...

	// 游戏标签管理器
	UGameplayTagsManager::Get().OnLastChanceToAddNativeTags().AddUObject(this, &UInventoryComponent::AddGameplayTags);
}

void UInventoryComponent::InitializeComponent()
{
	Super::InitializeComponent();

	if (GetOwner()->HasAuthority())
	{
		for (auto ItemClass : DefaultItems)
		{
			InventoryList.AddItem(ItemClass);
		}
	}

	if (InventoryList.GetItemsRef().Num())
	{
		// EquipItem(InventoryList.GetItemsRef()[0].ItemInstance->ItemStaticDataClass);

	}

	// 游戏标签绑定
	if (UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetOwner()))
	{
		// TODO 
		ASC->GenericGameplayEventCallbacks.FindOrAdd(UInventoryComponent::EquipItemActorTag).AddUObject(this, &UInventoryComponent::GameplayEventCallback);
		ASC->GenericGameplayEventCallbacks.FindOrAdd(UInventoryComponent::PickItemActorTag).AddUObject(this, &UInventoryComponent::GameplayEventCallback);
		ASC->GenericGameplayEventCallbacks.FindOrAdd(UInventoryComponent::OpenOrCloseInventoryWidgetTag).AddUObject(this, &UInventoryComponent::GameplayEventCallback);
		// ASC->GenericGameplayEventCallbacks.FindOrAdd(UInventoryComponent::EquipItemActorTag).AddUObject(this, &UInventoryComponent::GameplayEventCallback);
		// ASC->GenericGameplayEventCallbacks.FindOrAdd(UInventoryComponent::EquipItemActorTag).AddUObject(this, &UInventoryComponent::GameplayEventCallback);
		// ASC->GenericGameplayEventCallbacks.FindOrAdd(UInventoryComponent::EquipItemActorTag).AddUObject(this, &UInventoryComponent::GameplayEventCallback);
	}
}

bool UInventoryComponent::ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags)
{
	bool WroteSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);

	for (FInventoryListItem& Item : InventoryList.GetItemsRef())
	{
		UInventoryItemInstance* ItemInstance = Item.ItemInstance;

		if (IsValid(ItemInstance))
		{
			WroteSomething = Channel->ReplicateSubobject(ItemInstance, *Bunch, *RepFlags);
		}
	}
	return WroteSomething;
}

void UInventoryComponent::AddItem(TSubclassOf<UItemStaticData> InItemStaticDataClass)
{
	InventoryList.AddItem(InItemStaticDataClass);

	InventoryItemDataChanged.Broadcast(GetInventoryItemInstances());
}

void UInventoryComponent::AddItemInstance(UInventoryItemInstance* InItemInstance)
{
	if (GetOwner()->HasAuthority())
	{
		InventoryList.AddItem(InItemInstance);
		InventoryItemDataChanged.Broadcast(GetInventoryItemInstances());
	}
}

void UInventoryComponent::RemoveItem(TSubclassOf<UItemStaticData> InItemStaticDataClass)
{
	InventoryList.RemoveItem(InItemStaticDataClass);
	InventoryItemDataChanged.Broadcast(GetInventoryItemInstances());
}

// void UInventoryComponent::EquipItem(TSubclassOf<UItemStaticData> InItemStaticDataClass)
// {
// 	if (GetOwner()->HasAuthority())
// 	{
// 		// TODO ??? 为什么遍历"装备（Equipped）"?
// 		for (auto Item : InventoryList.GetItemsRef())
// 		{
// 			if (Item.ItemInstance->ItemStaticDataClass == InItemStaticDataClass)
// 			{
// 				// Item.ItemInstance->OnEquipped(GetOwner());
// 				CurrentItem = Item.ItemInstance;
// 				break;
// 			}
// 		}
// 	}
// }
//
// void UInventoryComponent::UnEquipItem(TSubclassOf<UItemStaticData> InItemStaticDataClass)
// {
// 	if (GetOwner()->HasAuthority())
// 	{
// 		// TODO ??? 为什么遍历"装备（Equipped）"?
// 		for (auto Item : InventoryList.GetItemsRef())
// 		{
// 			// CurrentItem->OnUnEquipped();
// 			CurrentItem = nullptr;
// 			break;
// 		}
// 	}
// }
//
// void UInventoryComponent::DropItem()
// {
// 	if (GetOwner()->HasAuthority())
// 	{
// 		if (IsValid(CurrentItem))
// 		{
// 			CurrentItem->OnDropped();
// 			RemoveItem(CurrentItem->ItemStaticDataClass);
// 			CurrentItem = nullptr;
// 		}
// 	}
// }

void UInventoryComponent::GameplayEventCallback(const FGameplayEventData* Payload)
{
	ENetRole NetRole = GetOwnerRole();

	if (Payload->EventTag == OpenOrCloseInventoryWidgetTag)
	{
		OpenOrCloseInventoryWidget();
		return;
	}
 
	// 多人网络处理
	if (NetRole == ROLE_Authority)
	{
		HandleGameplayEventInternal(*Payload);
	}
	else if (NetRole == ROLE_AutonomousProxy)
	{
		ServerHandleGameplayEvent(*Payload);
	}
}


void UInventoryComponent::AddGameplayTags()
{
	UGameplayTagsManager& TagsManager = UGameplayTagsManager::Get();

	// 向游戏标签管理器添加标签。
	UInventoryComponent::EquipItemActorTag = TagsManager.AddNativeGameplayTag("Event.Inventory.EquipItemActor", TEXT("Equip Item From Item Actor Event."));
	UInventoryComponent::PickItemActorTag = TagsManager.AddNativeGameplayTag("Event.Inventory.PickItemActor", TEXT("Pick Item From World."));
	UInventoryComponent::OpenOrCloseInventoryWidgetTag = TagsManager.AddNativeGameplayTag("Event.Inventory.OpenOrCloseInventoryWidget", TEXT("Open Or Close Inventory Widget."));
	
}

// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	InitializeComponent();
}

void UInventoryComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UInventoryComponent, InventoryList);
	DOREPLIFETIME(UInventoryComponent, CurrentItem);
}

void UInventoryComponent::OpenOrCloseInventoryWidget()
{
	if (const ACharacter* PlayerCharacter = Cast<ACharacter>(GetOwner()))
	{
		APlayerController* PC = Cast<APlayerController>(PlayerCharacter->GetController());
			
		if (PC)
		{
			if (!InventoryMainWidget)
			{
		
				{
					InventoryMainWidget = Cast<UMultiInventoryMainWidget>(CreateWidget(PC, InventoryWidgetClass));
					InventoryMainWidget->Init(this);
				}
			}
			
			if (InventoryMainWidget->IsInViewport())
			{
				CloseInventoryWidget();
			}
			else
			{
				OpenInventoryWidget();
			}
		}
	}
	
	
	
}

void UInventoryComponent::OpenInventoryWidget()
{
	if (!InventoryMainWidget)
	{
		return;
	}
	
	if (const ACharacter* PlayerCharacter = Cast<ACharacter>(GetOwner()))
	{
		APlayerController* PC = Cast<APlayerController>(PlayerCharacter->GetController());
			
		if (PC)
		{
			InventoryMainWidget->AddToViewport();
			PC->SetShowMouseCursor(true);
		}
	}
}

void UInventoryComponent::CloseInventoryWidget()
{
	if (!InventoryMainWidget)
	{
		return;
	}
	
	if (const ACharacter* PlayerCharacter = Cast<ACharacter>(GetOwner()))
	{
		APlayerController* PC = Cast<APlayerController>(PlayerCharacter->GetController());
			
		if (PC)
		{
			InventoryMainWidget->RemoveFromParent();
			PC->SetShowMouseCursor(false);
		}
	}
}

// Called every frame
void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
	// const bool bShowDebug = CVarDebugInventoryComponent.GetValueOnGameThread() == 0 ? false : true;
	//
	// if (bShowDebug)
	// {
	// 	for (auto Item : InventoryList.GetItemsRef())
	// 	{
	// 		UInventoryItemInstance* ItemInstance = Item.ItemInstance;
	//
	// 		const UItemStaticData* ItemStaticData = ItemInstance->GetItemStaticData();
	// 		if (IsValid(ItemInstance) && IsValid(ItemStaticData))
	// 		{
	// 			GEngine->AddOnScreenDebugMessage(-1, 0, FColor::Blue, FString::Printf(TEXT("Item: %s"), *ItemStaticData->Name.ToString()));
	// 		}
	// 	}
	// }
}

TArray<UInventoryItemInstance*> UInventoryComponent::GetInventoryItemInstances()
{
	TArray<UInventoryItemInstance*> InventoryItemInstances;
	for (int Index = 0; Index < InventoryList.Items.Num(); ++Index)
	{
		InventoryItemInstances.Add(InventoryList.Items[Index].ItemInstance);
	}
	
	return InventoryItemInstances;
}

