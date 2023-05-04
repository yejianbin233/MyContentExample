// Fill out your copyright notice in the Description page of Project Settings.


#include "LoadingScreenWidget.h"

#include "Animation/UMGSequenceTickManager.h"
#include "Extensions/UserWidgetExtension.h"

// extern TAutoConsoleVariable<int32> CVarUserWidgetUseParallelAnimation;

void ULoadingScreenWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	// If this ensure is hit it is likely UpdateCanTick as not called somewhere
	if(ensureMsgf(GetDesiredTickFrequency() != EWidgetTickFrequency::Never, TEXT("SObjectWidget and UUserWidget have mismatching tick states or UUserWidget::NativeTick was called manually (Never do this)")))
	{
		GInitRunaway();

		TSubclassOf<UUserWidgetExtension> InExtensionType;
		TArray<UUserWidgetExtension*> Result = GetExtensions(InExtensionType);
		// Extension can be added while ticking another extension.
		//This loop does guarantee that they will all be updated this frame, if it's the case,  but it will not crash.
		for (int32 Index = 0; Index < Result.Num(); ++Index)
		{
			Result[Index]->Tick(MyGeometry, InDeltaTime);
		}

#if WITH_EDITOR
		const bool bTickAnimations = !IsDesignTime();
#else
		const bool bTickAnimations = true;
#endif
		if (true)
		{
			if (AnimationTickManager)
			{
				AnimationTickManager->OnWidgetTicked(this);
			}

			if (true)
			{
				TickActionsAndAnimation(InDeltaTime);
				PostTickActionsAndAnimation(InDeltaTime);
			}
			// else: the TickManager object will tick all animations at once.

			UWorld* World = GetWorld();
			if (World)
			{
				// Update any latent actions we have for this actor
				World->GetLatentActionManager().ProcessLatentActions(this, InDeltaTime);
			}
		}

		if (bHasScriptImplementedTick)
		{
			Tick(MyGeometry, InDeltaTime);
		}
	}
}
