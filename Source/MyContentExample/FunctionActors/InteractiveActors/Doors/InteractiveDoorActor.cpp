// Fill out your copyright notice in the Description page of Project Settings.


#include "FunctionActors/InteractiveActors/Doors/InteractiveDoorActor.h"

void AInteractiveDoorActor::Interactive(int32 InteractiveId)
{
	Super::Interactive(InteractiveId);

	if (EDoorInteractiveType::RevolvinDoor == InteractiveId)
	{
		OpenRevolvinDoor();
	}
	else if (EDoorInteractiveType::RevolvinDoor == InteractiveId)
	{
		OpenRisingDoor();
	}
}

void AInteractiveDoorActor::OpenRisingDoor_Implementation()
{
	
}

void AInteractiveDoorActor::OpenRevolvinDoor_Implementation()
{
	
}
