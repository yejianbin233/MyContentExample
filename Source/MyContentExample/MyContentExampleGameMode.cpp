// Copyright Epic Games, Inc. All Rights Reserved.

#include "MyContentExampleGameMode.h"
#include "MyContentExampleCharacter.h"
#include "UObject/ConstructorHelpers.h"

AMyContentExampleGameMode::AMyContentExampleGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
