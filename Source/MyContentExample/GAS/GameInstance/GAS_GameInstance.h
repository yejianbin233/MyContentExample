// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"

#include "Runtime/UMG/Public/UMG.h"
#include "Runtime/UMG/Public/UMGStyle.h"
#include "Runtime/UMG/Public/Slate/SObjectWidget.h"
#include "Runtime/UMG/Public/IUMGModule.h"
#include "Runtime/UMG/Public/Blueprint/UserWidget.h"

#include "Runtime/MoviePlayer/Public/MoviePlayer.h"
#include "Runtime/SlateCore/Public/Widgets/SWidget.h"

#include "GAS_GameInstance.generated.h"

/**
 * 
 */
UCLASS()
class MYCONTENTEXAMPLE_API UGAS_GameInstance : public UGameInstance
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category="LoadingScreen", meta=(AllowPrivateAccess=true))
	TSubclassOf<UUserWidget> LoadingScreenWidgetClass;

	UPROPERTY(BlueprintReadOnly, Category="LoadingScreen", meta=(AllowPrivateAccess=true))
	UUserWidget* LoadingScreenWidget;

	UPROPERTY(EditDefaultsOnly, Category="LoadingScreen", meta=(AllowPrivateAccess=true))
	bool bAutoCompleteWhenLoadingCompletes;

	UPROPERTY(EditDefaultsOnly, Category="LoadingScreen", meta=(AllowPrivateAccess=true))
	float MinimumLoadingScreenDisplayTime;
	
	UPROPERTY(EditDefaultsOnly, Category="LoadingScreen", meta=(AllowPrivateAccess=true))
	bool bWaitForManualStop;

	UPROPERTY(EditDefaultsOnly, Category="LoadingScreen", meta=(AllowPrivateAccess=true))
	bool bMoviesAreSkippable;

	UPROPERTY(EditDefaultsOnly, Category="LoadingScreen", meta=(AllowPrivateAccess=true))
	TEnumAsByte<EMoviePlaybackType> PlaybackType;

public:
	virtual void Init() override;

	void BeginLoadingScreen(const FString& MapName);
	void EndLoadingScreen(UWorld* LoadedWorld);

	UFUNCTION(BlueprintNativeEvent, Category="LoadingScreen")
	void BeginLoadingScreenHandle();

	UFUNCTION(BlueprintNativeEvent, Category="LoadingScreen")
	void EndLoadingScreenHandle();
};
