// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/Actors/ItemActor.h"

#include "Engine/ActorChannel.h"
#include "Inventory/InventoryItemInstance.h"
#include "Net/UnrealNetwork.h"
#include "../BlueprintFunctionLibrarys/BLF_Inventory.h"

extern TAutoConsoleVariable<int32> CVarDebugInventoryComponent;

// Sets default values
AItemActor::AItemActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	bReplicates = true;
    AActor::SetReplicateMovement(true);
	
}

void AItemActor::OnEquipped()
{
	
}

void AItemActor::OnUnequipped()
{
	
}

void AItemActor::OnDropped()
{
	GetRootComponent()->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
}

void AItemActor::Init(UInventoryItemInstance* InItemInstance)
{
	ItemInstance = InItemInstance;

	const int32 IsDebugInventoryComponent = CVarDebugInventoryComponent.GetValueOnGameThread();
	
	if (IsDebugInventoryComponent)
	{
		if (!ItemInstance)
		{
			UE_LOG(LogTemp, Warning, TEXT("%s Init Fail: ItemInstance is null."), *GetName())
		}

		if (ItemInstance && UBLF_Inventory::GetItemStaticData(ItemStaticDataClass) != ItemInstance->GetItemStaticData())
		{
			UE_LOG(LogTemp, Warning, TEXT("%s Init Fail: ItemStaticDataClass not match."), *GetName())
		}
	}
	
	InitInternal();
}

UInventoryItemInstance* AItemActor::GetInventoryItemInstance_Implementation()
{
	return nullptr;
}

// Called when the game starts or when spawned
void AItemActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void AItemActor::InitInternal()
{
	
}

void AItemActor::OnRep_ItemInstance(UInventoryItemInstance* InItemInstance)
{
	if (IsValid(ItemInstance) && !IsValid(InItemInstance))
	{
		InitInternal();
	}
}

bool AItemActor::ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags)
{
	bool WroteSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);

	if (IsValid(ItemInstance))
	{
		 WroteSomething = Channel->ReplicateSubobject(ItemInstance, *Bunch, *RepFlags);
	}

	return WroteSomething;
}

void AItemActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AItemActor, ItemInstance);
}

// Called every frame
void AItemActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

