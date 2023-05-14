// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "2DHorizontal/GameDatas/MovementType.h"
#include "Hori2DCharacterMovementComponent.generated.h"

class UMovementStateInstanceBase;
class UInputAction;
class UInputMappingContext;
class APaperZDHorizontalCharacter;
/**
 * 
 */
UCLASS()
class MYCONTENTEXAMPLE_API UHori2DCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()


	/* 玩家输入 */
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* AttackAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* DodgeAction;
	
	/** 玩家输入按键检测 */
	
	UPROPERTY(EditDefaultsOnly, Category="Enhance Input|Hold")
	UInputAction* W;

	UPROPERTY(EditDefaultsOnly, Category="Enhance Input|Hold")
	UInputAction* S;

	UPROPERTY(EditDefaultsOnly, Category="Enhance Input|Hold")
	UInputAction* A;

	UPROPERTY(EditDefaultsOnly, Category="Enhance Input|Hold")
	UInputAction* D;

	UPROPERTY(EditDefaultsOnly, Category="Enhance Input|Hold")
	UInputAction* J;

	UPROPERTY(EditDefaultsOnly, Category="Enhance Input|Hold")
	UInputAction* K;

	UPROPERTY(EditDefaultsOnly, Category="Enhance Input|Hold")
	UInputAction* L;

	UPROPERTY(BlueprintReadOnly, Category="Key States", meta=(AllowPrivateAccess))
	EPlayerInputKeyState W_KeyState;
	UPROPERTY(BlueprintReadOnly, Category="Key States", meta=(AllowPrivateAccess))
	EPlayerInputKeyState S_KeyState;
	UPROPERTY(BlueprintReadOnly, Category="Key States", meta=(AllowPrivateAccess))
	EPlayerInputKeyState A_KeyState;
	UPROPERTY(BlueprintReadOnly, Category="Key States", meta=(AllowPrivateAccess))
	EPlayerInputKeyState D_KeyState;
	UPROPERTY(BlueprintReadOnly, Category="Key States", meta=(AllowPrivateAccess))
	EPlayerInputKeyState J_KeyState;
	UPROPERTY(BlueprintReadOnly, Category="Key States", meta=(AllowPrivateAccess))
	EPlayerInputKeyState K_KeyState;
	UPROPERTY(BlueprintReadOnly, Category="Key States", meta=(AllowPrivateAccess))
	EPlayerInputKeyState L_KeyState;
	
	/** 玩家输入按键检测 */

	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess))
	UMovementStateInstanceBase* MovementStateInstance;
	
public:

	void InitPlayerInputKeyListen(APaperZDHorizontalCharacter* PlayerCharacter);

	UFUNCTION(BlueprintCallable)
	void InputHandle(EMovementStateType InMovementState);
	
protected:

	/* 监听玩家输入按键状态 */
	void OnW_Pressed();
	void OnW_Relax();
	void OnW_Hold();

	void OnS_Pressed();
	void OnS_Relax();
	void OnS_Hold();

	void OnA_Pressed();
	void OnA_Relax();
	void OnA_Hold();

	void OnD_Pressed();
	void OnD_Relax();
	void OnD_Hold();

	void OnJ_Pressed();
	void OnJ_Relax();
	void OnJ_Hold();
	
	void OnK_Pressed();
	void OnK_Relax();
	void OnK_Hold();

	void OnL_Pressed();
	void OnL_Relax();
	void OnL_Hold();

	void ShowDebugInfo(FString KeyName, EPlayerInputKeyState KeyState);
	/* 监听玩家输入按键状态 */

	bool CanRotateSprite();
	
	
};
