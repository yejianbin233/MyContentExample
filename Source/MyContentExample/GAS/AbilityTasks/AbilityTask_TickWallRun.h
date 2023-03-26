// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "AbilityTask_TickWallRun.generated.h"


class UCharacterMovementComponent;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWallRunWallSideDeternenedDelegate, bool, bLeftSide);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnWallRunFinishedDelegate);
/**
 * 
 */
UCLASS()
class MYCONTENTEXAMPLE_API UAbilityTask_TickWallRun : public UAbilityTask
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintAssignable)
	FOnWallRunFinishedDelegate OnFinished;

	UPROPERTY(BlueprintAssignable)
	FOnWallRunWallSideDeternenedDelegate OnWallSideDeternened;

	// 必须定义工厂函数，用于实例化 AbilityTask
	UFUNCTION(BlueprintCallable, Category="Ability|Tasks", meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	static UAbilityTask_TickWallRun* CreateWallRunTask(UGameplayAbility* OwningAbility,
		ACharacter* InCharacterOwner,
		UCharacterMovementComponent* InCharacterMovementComponent,
		TArray<TEnumAsByte<EObjectTypeQuery>> InTraceObjectTypes);

	virtual void Activate() override;

	virtual void OnDestroy(bool bInOwnerFinished) override;

	virtual void TickTask(float DeltaTime) override;

protected:

	UCharacterMovementComponent* CharacterMovementComponent = nullptr;

	ACharacter* CharacterOwner = nullptr;

	TArray<TEnumAsByte<EObjectTypeQuery>> TraceObjectTypes;

	bool FindRunnableWall(FHitResult& OnWallHitResult);

	bool IsWallOnTheLeft(const FHitResult& InWallHitResult) const;
};
