// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "AbilitySystemInterface.h"
#include "GameplayTagContainer.h"
#include "GAS/Components/GAS_CharacterMovementComponent.h"
#include "GAS/DataAssets/GAS_CharacterAnimDataAsset.h"
#include "GAS/GameType/GAS_GameTypes.h"
#include "GAS_Character.generated.h"

class UGAS_CharacterMovementComponent;
class UGAS_MotionWarpingComponent;
struct FGameplayTag;
class UGAS_CharacterAnimDataAsset;
class UGAS_AbilitySystemComponentBase;
class UGAS_AttributeSetBase;

class UGameplayEffect;
class UGameplayAbility;
struct FGameplayEffectContextHandle;

class UGAS_CharacterDataAsset;

UCLASS(config=Game)
class AGAS_Character : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;
	
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* CrouchAction;

public:

	AGAS_Character(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void PostInitializeComponents() override;
	/*============ GAS ============*/
	// 给自身应用 GameplayEffect
	bool ApplyGameplayEffecToSelf(TSubclassOf<UGameplayEffect> Effect, FGameplayEffectContextHandle InEffectContextHandle);

	UFUNCTION(BlueprintCallable, Category="GAS")
	void SetGASCharacterData(const FGASCharacterData& InCharacterData);
	/*============ GAS ============*/

	virtual void PawnClientRestart() override;

protected:

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	/*============ GAS ============*/

	// 授予技能
	void GiveAbilities();
	// 应用初始 GameplayEffect
	void ApplyStartupGameplayEffects();
	
	// 初始化 GAS 属性集
	virtual void InitFromCharacterData(const FGASCharacterData& InCharacterData, bool bFromReplication = false /*用于判断 InCharacterData 是否是网络复制传入*/);

	UFUNCTION()
	virtual void OnRep_CharacterData(FGASCharacterData InCharacterData);

	// 玩家输入调用该函数，用来拾取物品（通过游戏标签 + 游戏事件）来实现通信。
	void OnEquipItemActor();
	/*============ GAS ============*/

	// 当服务器重新拥有玩家的控制器时，执行的逻辑
	virtual void PossessedBy(AController* NewController) override;
	// 当客户端玩家状态更新时，执行的逻辑
	virtual void OnRep_PlayerState() override;

	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// To add mapping context
	virtual void BeginPlay();

	virtual void StartJump(const FInputActionValue& Value);

	virtual void StopJumping() override;

	virtual void Landed(const FHitResult& Hit) override;

	void OnCrouchActionStarted(const FInputActionValue& Value);

	void OnCrouchActionEnded(const FInputActionValue& Value);

	virtual void OnStartCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust) override;

	virtual void OnEndCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust) override;

protected:
	/*============ GAS ============*/
	
	// GAS 系统组件
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="GAS")
	UGAS_AbilitySystemComponentBase* AbilitySystemComponent;

	// 属性集，Transient 修饰令其无法保存、加载。
	UPROPERTY(Transient)
	UGAS_AttributeSetBase* AttributeSet;

	UPROPERTY(ReplicatedUsing="OnRep_CharacterData")
	FGASCharacterData CharacterData;

	UPROPERTY(EditDefaultsOnly)
	UGAS_CharacterDataAsset* CharacterDataAsset;

	UPROPERTY(EditDefaultsOnly)
	UGAS_CharacterAnimDataAsset* CharacterAnimDataAsset;

	UPROPERTY(BlueprintReadOnly)
	class UGAS_FootstepsComponent* FootstepsComponent;

	UPROPERTY(EditDefaultsOnly)
	FGameplayTag JumpEventTag;

	UPROPERTY(EditDefaultsOnly)
	FGameplayTag GA_JumpTag;

	UPROPERTY(EditDefaultsOnly)
	FGameplayTagContainer InAirTags;

	UPROPERTY(EditDefaultsOnly)
	FGameplayTagContainer CrouchTags;

	UPROPERTY(EditDefaultsOnly, Category="GameplayEffect")
	TSubclassOf<UGameplayEffect> CrouchStateEffect;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="MotionWarp")
	UGAS_MotionWarpingComponent* MotionWarpingComponent;
	/*============ GAS ============*/

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UFUNCTION(BlueprintCallable, Category="GAS")
	FORCEINLINE FGASCharacterData GetGASCharacterData() const { return CharacterData;};

	UFUNCTION(BlueprintCallable, Category="GAS")
	FORCEINLINE UGAS_CharacterAnimDataAsset* GetGASCharacterAnimData() const { return CharacterAnimDataAsset; };

	FORCEINLINE UGAS_FootstepsComponent* GetFootstepsComponent() const { return FootstepsComponent; };

	FORCEINLINE UGAS_MotionWarpingComponent* GetMotionWarpingComponent() const { return MotionWarpingComponent; };

	FORCEINLINE UGAS_CharacterMovementComponent* GetGasCharacterMovementComponent() const { return Cast<UGAS_CharacterMovementComponent>(GetCharacterMovement()); };
};

