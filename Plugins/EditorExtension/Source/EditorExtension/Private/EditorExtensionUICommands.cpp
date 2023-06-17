// Fill out your copyright notice in the Description page of Project Settings.


#include "EditorExtensionUICommands.h"

#define LOCTEXT_NAMESPACE "FEditorExtensionModule"

void FEditorExtensionUICommands::RegisterCommands()
{
	// 使用工具宏
	// note：需要添加模块的本地化内容 #define LOCTEXT_NAMESPACE "FEditorExtensionModule"
	UI_COMMAND(LockActorsSelection,
		"Lock Actor Selection",
		"Lock actor selection in level, actor can no longer be selected.",
		EUserInterfaceActionType::Button, // 按钮
		FInputChord(EKeys::W, EModifierKey::Alt) /*快捷键组合方式，需要注意不要使用已使用的组合快捷键*/);

	UI_COMMAND(UnlockActorsSelection,
		"Unlock Actor Selection",
		"Remove selection lock on all actors.",
		EUserInterfaceActionType::Button, // 按钮
		FInputChord(EKeys::W, EModifierKey::Alt | EModifierKey::Shift) /*快捷键组合方式，需要注意不要使用已使用的组合快捷键*/);
}

#undef LOCTEXT_NAMESPACE