// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Framework/Commands/Commands.h"

/**
 * 自定义快捷键
 */

class FEditorExtensionUICommands : public TCommands<FEditorExtensionUICommands>
{
public:
	FEditorExtensionUICommands() : TCommands<FEditorExtensionUICommands>(TEXT("EditorExtensionUICommands"),
		FText::FromString(TEXT("Editor Extension UICommands")),
		NAME_None,
		TEXT("EditorExtensionUICommands")
		){};

	virtual void RegisterCommands() override;

	// 锁定 Actor 功能快捷键信息
	TSharedPtr<FUICommandInfo> LockActorsSelection;
	// 解锁 Actor
	TSharedPtr<FUICommandInfo> UnlockActorsSelection;
};
