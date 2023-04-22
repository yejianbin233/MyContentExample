// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/InventoryItemInstance.h"

#include "Actors/ItemActor.h"
#include "GameFramework/Character.h"
#include "Net/UnrealNetwork.h"

void UInventoryItemInstance::Init(TSubclassOf<UItemStaticData> InItemStaticDataClass)
{
	ItemStaticDataClass = InItemStaticDataClass;
}

// void UInventoryItemInstance::OnRep_Equipped()
// {
// 	
// }

// void UInventoryItemInstance::OnEquipped(AActor* InOwner)
// {
// 	if (UWorld* World = GetWorld())
// 	{
//
// 		const UItemStaticData* StaticData = GetItemStaticData();
// 		
// 		FTransform Transform;
// 		ItemActor = World->SpawnActorDeferred<AItemActor>(GetItemStaticData()->ItemActorClass, Transform);
//
// 		ItemActor->Init(this);
//
// 		ACharacter* Character = Cast<ACharacter>(InOwner);
//
// 		USkeletalMeshComponent* SkeletalMeshComponent = Character ? Character->GetMesh() : nullptr;
//
// 		if (SkeletalMeshComponent)
// 		{
// 			ItemActor->AttachToComponent(SkeletalMeshComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale, StaticData->AttachmentSocket);
// 		}
// 		ItemActor->FinishSpawning(Transform);
// 	}
// }
//
// void UInventoryItemInstance::OnUnEquipped()
// {
// 	if (ItemActor)
// 	{
// 		ItemActor->OnUnequipped();
// 		ItemActor->Destroy();
// 		ItemActor = nullptr;	
// 	}
// }

void UInventoryItemInstance::OnDropped()
{
	if (ItemActor)
	{
		ItemActor->OnDropped();
		ItemActor = nullptr;
	}
}

void UInventoryItemInstance::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UInventoryItemInstance, ItemStaticDataClass);
	// DOREPLIFETIME(UInventoryItemInstance, bEquipped);
	DOREPLIFETIME(UInventoryItemInstance, ItemActor);
}
