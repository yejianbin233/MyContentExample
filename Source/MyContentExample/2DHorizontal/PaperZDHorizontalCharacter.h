// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "PaperZDCharacter.h"
#include "PaperZDHorizontalCharacter.generated.h"

struct FInputActionValue;
class USpringArmComponent;
class UCameraComponent;
/**
 * 
 */
UCLASS()
class MYCONTENTEXAMPLE_API APaperZDHorizontalCharacter : public APaperZDCharacter
{
	GENERATED_BODY()

public:

protected:
	
	UPROPERTY(EditDefaultsOnly, Category="Camera")
	UCameraComponent* CameraComponent;

	UPROPERTY(EditDefaultsOnly, Category="Camera")
	USpringArmComponent* CameraSprintArmComponent;
	
public:
	APaperZDHorizontalCharacter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	void ExecMoveAction(const FInputActionValue& Value);
    
	void ExecJumpAction(const FInputActionValue& Value);
	
protected:

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	virtual void PawnClientRestart() override;

	virtual void PossessedBy(AController* NewController) override;
};
