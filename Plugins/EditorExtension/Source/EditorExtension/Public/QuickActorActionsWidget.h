// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EditorUtilityWidgetBlueprint.h"
#include "QuickActorActionsWidget.generated.h"


UENUM(BlueprintType)
enum class E_DuplicationAxis : uint8
{
	EDA_XAxis UMETA(DisplayName = "X Axis"),
	EDA_YAxis UMETA(DisplayName = "Y Axis"),
	EDA_ZAxis UMETA(DisplayName = "Z Axis"),
	EDA_Max UMETA(DisplayName = "Default Max"),
};

USTRUCT(BlueprintType)
struct FRandomActorRotation
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bRandomizeRotYaw = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(EditCondition = "bRandomizeRotYaw"))
	float RotYawMin = -45.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(EditCondition = "bRandomizeRotYaw"))
	float RotYawMax = 45.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bRandomizeRotPitch = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(EditCondition = "bRandomizeRotPitch"))
	float RotPitchMin = -45.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(EditCondition = "bRandomizeRotPitch"))
	float RotPitchMax = 45.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bRandomizeRotRoll = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(EditCondition = "bRandomizeRotRoll"))
	float RotRollMin = -45.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(EditCondition = "bRandomizeRotRoll"))
	float RotRollMax = 45.0f;
};
/**
 * 
 */
UCLASS()
class EDITOREXTENSION_API UQuickActorActionsWidget : public UEditorUtilityWidget
{
	GENERATED_BODY()

public:

#pragma region ActorsSelect
	// 选择所有名称相似的 Actor
	UFUNCTION(BlueprintCallable)
	void SelectAllActorsWithSimilarName();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ActorBatchSelection")
	TEnumAsByte<ESearchCase::Type> SearchCase = ESearchCase::Type::IgnoreCase;
	
#pragma endregion

#pragma region ActorsDuplication

	// 复制所选 Actors
	UFUNCTION(BlueprintCallable)
	void DuplicateActors();

	// 复制 Actor 的数量
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ActorBatchDuplication")
	int32 NumberOdDuplicates = 5;

	// 复制的 Actor 的偏移距离
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ActorBatchDuplication")
	float OffsetDist = 300.0f;

	E_DuplicationAxis AxisForDuplication = E_DuplicationAxis::EDA_XAxis;
	
#pragma endregion

#pragma region RandomizeActorTransform

	UFUNCTION(BlueprintCallable)
	void RandomizeActorTransform();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="RandomizeActorTransform")
	FRandomActorRotation RandomActorRotation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="RandomizeActorTransform")
	bool bRandomizeScale = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="RandomizeActorTransform", meta=(EditCondition = "bRandomizeScale"))
	float ScaleMin = 0.8f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="RandomizeActorTransform", meta=(EditCondition = "bRandomizeScale"))
	float ScaleMax = 0.8f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="RandomizeActorTransform")
	bool bRandomizeOffset = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="RandomizeActorTransform", meta=(EditCondition = "bRandomizeOffset"))
	float OffsetMin = 0.8f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="RandomizeActorTransform", meta=(EditCondition = "bRandomizeOffset"))
	float OffsetMax = 0.8f;
	
#pragma endregion 

private:

	UPROPERTY()
	class UEditorActorSubsystem* EditorActorSubsystem;

	bool GetEditorActorSubsystem();
};
