// Fill out your copyright notice in the Description page of Project Settings.


#include "FunctionActors/InteractiveActors/Doors/InteractiveDoorActor.h"

void AInteractiveDoorActor::Interactive(int32 InteractiveId)
{
	Super::Interactive(InteractiveId);

	if (EDoorInteractiveType::RevolvinDoor == InteractiveId)
	{
		OpenRevolvinDoor();
	}
	else if (EDoorInteractiveType::RisingDoor == InteractiveId)
	{
		OpenRisingDoor();
	}
}

int32 AInteractiveDoorActor::GetInteractiveIdByDoorInteractiveType(EDoorInteractiveType InteractiveType)
{
	if (EDoorInteractiveType::RevolvinDoor == InteractiveType)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}

void AInteractiveDoorActor::OpenRisingDoor_Implementation()
{
	
}

void AInteractiveDoorActor::OpenRevolvinDoor_Implementation()
{
	
}
