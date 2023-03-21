// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GAS/GameType/GAS_GameTypes.h"
#include "GAS_CharacterAnimDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class MYCONTENTEXAMPLE_API UGAS_CharacterAnimDataAsset : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly)
	FGASCharacterAnimData CharacterAnimData;
};
