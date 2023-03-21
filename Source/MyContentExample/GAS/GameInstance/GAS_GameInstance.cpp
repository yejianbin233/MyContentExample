// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/GameInstance/GAS_GameInstance.h"
#include "AbilitySystemGlobals.h"

void UGAS_GameInstance::Init()
{
	Super::Init();
	// 初始化 GAS 全局数据。
	UAbilitySystemGlobals::Get().InitGlobalData();
}
