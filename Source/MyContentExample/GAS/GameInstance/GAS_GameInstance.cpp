// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/GameInstance/GAS_GameInstance.h"
#include "AbilitySystemGlobals.h"
#include "MoviePlayer.h"
#include "Blueprint/UserWidget.h"

void UGAS_GameInstance::Init()
{
	Super::Init();
	// 初始化 GAS 全局数据。
	UAbilitySystemGlobals::Get().InitGlobalData();

	// 关卡加载界面
	FCoreUObjectDelegates::PreLoadMap.AddUObject(this, &UGAS_GameInstance::BeginLoadingScreen);
	FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(this, &UGAS_GameInstance::EndLoadingScreen);
}

void UGAS_GameInstance::BeginLoadingScreen(const FString& MapName)
{
	// 虚幻引擎自带的加载屏幕类型，可填充加载屏幕的相关数据。
	FLoadingScreenAttributes LoadingScreenAttributes;

	LoadingScreenWidget = CreateWidget<UUserWidget>(this, LoadingScreenWidgetClass);

	if (!LoadingScreenWidget)
	{
		UE_LOG(LogTemp, Log, TEXT("LoadingScreen Widget Is Null!"));
		return;
	}

	TSharedPtr<SWidget> WidgetPtr = LoadingScreenWidget->TakeWidget();
	LoadingScreenAttributes.WidgetLoadingScreen = WidgetPtr;
	LoadingScreenAttributes.bAutoCompleteWhenLoadingCompletes = bAutoCompleteWhenLoadingCompletes;
	LoadingScreenAttributes.MinimumLoadingScreenDisplayTime = MinimumLoadingScreenDisplayTime;
	LoadingScreenAttributes.bWaitForManualStop = bWaitForManualStop;
	LoadingScreenAttributes.bMoviesAreSkippable = bMoviesAreSkippable;
	LoadingScreenAttributes.PlaybackType = PlaybackType;
	// for (int i=0; i < MoviePaths.Num(); ++i)
	// {
	// 	LoadingScreenAttributes.MoviePaths.Add(MoviePaths[i]);
	// }

	LoadingScreenAttributes.MoviePaths.Add("DepthSea");
	LoadingScreenAttributes.MoviePaths.Add("RaphaelLAcoste");
	GetMoviePlayer()->SetupLoadingScreen(LoadingScreenAttributes);
	BeginLoadingScreenHandle();
}

void UGAS_GameInstance::EndLoadingScreen(UWorld* LoadedWorld)
{
	EndLoadingScreenHandle();
}

void UGAS_GameInstance::BeginLoadingScreenHandle_Implementation()
{
	
}

void UGAS_GameInstance::EndLoadingScreenHandle_Implementation()
{
	
}
