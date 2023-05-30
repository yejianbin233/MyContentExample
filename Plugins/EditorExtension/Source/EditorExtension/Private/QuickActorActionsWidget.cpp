// Fill out your copyright notice in the Description page of Project Settings.


#include "QuickActorActionsWidget.h"

#include "DebugHeader.h"
#include "Subsystems/EditorActorSubsystem.h"

#pragma region ActorsSelect

void UQuickActorActionsWidget::SelectAllActorsWithSimilarName()
{
	if (!GetEditorActorSubsystem()) return;

	TArray<AActor*> SelectedActors = EditorActorSubsystem->GetSelectedLevelActors();
	uint32 SelectionCounter = 0;

	if (SelectedActors.Num() == 0)
	{
		DebugHeader::ShowNotifyInfo(TEXT("No Actor selected"));
		return ;
	}

	if (SelectedActors.Num() > 1)
	{
		DebugHeader::ShowNotifyInfo(TEXT("You can only select one actor"));
		return ;
	}

	const FString SelectedActorName = SelectedActors[0]->GetActorLabel();
	const FString NameToSearch = SelectedActorName.LeftChop(4);

	TArray<AActor*> AllLevelActors = EditorActorSubsystem->GetAllLevelActors();

	for (AActor* ActorInLevel: AllLevelActors)
	{
		if (!ActorInLevel) continue;

		if (ActorInLevel->GetActorLabel().Contains(NameToSearch, SearchCase))
		{
			// 设置在编辑器中名称相似 Actor 的选中状态 -> 选中。
			EditorActorSubsystem->SetActorSelectionState(ActorInLevel, true);

			++SelectionCounter;
		}
	}

	if (SelectionCounter > 0)
	{
		DebugHeader::ShowNotifyInfo(TEXT("Successfully selected ") + FString::FromInt(SelectionCounter) + TEXT(" Actors"));
	}
	else
	{
		DebugHeader::ShowNotifyInfo(TEXT("No actor with similar name found"));
	}
}

void UQuickActorActionsWidget::DuplicateActors()
{
	if (!GetEditorActorSubsystem()) return ;

	
	TArray<AActor*> SelectedActors = EditorActorSubsystem->GetSelectedLevelActors();

	uint32 Counter = 0;

	if (SelectedActors.Num() == 0)
	{
		DebugHeader::ShowNotifyInfo(TEXT("No actor selected"));
		return ;
	}

	if (SelectedActors.Num() <= 0)
	{
		DebugHeader::ShowNotifyInfo(TEXT("Did not specify a number of duplications."));
		return ;
	}

	if (OffsetDist == 0)
	{
		DebugHeader::ShowNotifyInfo(TEXT("Did not specify a number of OffsetDist."));
		return ;
	}

	for (AActor* SelectedActor : SelectedActors)
	{
		if (!SelectedActor) continue;

		for (int32 i=0; i<NumberOdDuplicates; ++i)
		{
			AActor* DuplicatedActor = EditorActorSubsystem->DuplicateActor(SelectedActor, SelectedActor->GetWorld());

			if (!DuplicatedActor) continue;

			const float DuplicationOffsetDist = (i+1) * OffsetDist;
			
			switch(AxisForDuplication) {
				case E_DuplicationAxis::EDA_XAxis:
					DuplicatedActor->AddActorLocalOffset(FVector(DuplicationOffsetDist, 0.0f, 0.0f));
					break;
				case E_DuplicationAxis::EDA_YAxis:
					DuplicatedActor->AddActorLocalOffset(FVector(0.0f, DuplicationOffsetDist, 0.0f));
					break;
				case E_DuplicationAxis::EDA_ZAxis:
					DuplicatedActor->AddActorLocalOffset(FVector(0.0f, 0.0f, DuplicationOffsetDist));
					break;
				case E_DuplicationAxis::EDA_Max:
					break;
				default: ;
			}
			
			EditorActorSubsystem->SetActorSelectionState(DuplicatedActor, true);
			++Counter;
		}

		if (Counter > 0)
		{
			DebugHeader::ShowNotifyInfo(TEXT("Successfully duplicated ") + FString::FromInt(Counter) + TEXT(" actor."));
		}
	}
}

#pragma endregion 

#pragma region RandomizeActorTransform

void UQuickActorActionsWidget::RandomizeActorTransform()
{
	const bool ConditionNotSet = !(RandomActorRotation.bRandomizeRotYaw
		|| RandomActorRotation.bRandomizeRotPitch
		|| RandomActorRotation.bRandomizeRotRoll);

	int32 Counter = 0;

	if (ConditionNotSet)
	{
		DebugHeader::ShowNotifyInfo(TEXT("No variation condition specified."));
		
		return ;
	}
	
	if(!GetEditorActorSubsystem()) return ;

	TArray<AActor*> SelectedActors = EditorActorSubsystem->GetSelectedLevelActors();

	if (SelectedActors.Num() == 0)
	{
		DebugHeader::ShowNotifyInfo(TEXT("No actor selected."));
		
		return ;
	}

	for (AActor* SelectedActor : SelectedActors)
	{
		if (!SelectedActor) continue ;

		if (RandomActorRotation.bRandomizeRotYaw)
		{
			const float RandomRotYawValue = FMath::RandRange(RandomActorRotation.RotYawMin, RandomActorRotation.RotYawMax);

			SelectedActor->AddActorWorldRotation(FRotator(0.0f, RandomRotYawValue, 0.0f));
		}

		if (RandomActorRotation.bRandomizeRotPitch)
		{
			const float RandomRotPitchValue = FMath::RandRange(RandomActorRotation.RotPitchMin, RandomActorRotation.RotPitchMax);

			SelectedActor->AddActorWorldRotation(FRotator(RandomRotPitchValue, 0.0f, 0.0f));
		}

		if (RandomActorRotation.bRandomizeRotRoll)
		{
			const float RandomRotRollValue = FMath::RandRange(RandomActorRotation.RotRollMin, RandomActorRotation.RotRollMax);

			SelectedActor->AddActorWorldRotation(FRotator(0.0f, 0.0f, RandomRotRollValue));
		}

		++Counter;
	}

	if (Counter > 0)
	{
		DebugHeader::ShowNotifyInfo(TEXT("Successfully set ") + FString::FromInt(Counter) + TEXT(" actors"));
	}
}

#pragma endregion 

bool UQuickActorActionsWidget::GetEditorActorSubsystem()
{
	if (!EditorActorSubsystem)
	{
		EditorActorSubsystem = GEditor->GetEditorSubsystem<UEditorActorSubsystem>();
	}
	
	return EditorActorSubsystem != nullptr;
}
