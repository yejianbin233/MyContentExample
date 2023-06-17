// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Styling/SlateStyle.h"


class FEditorExtensionStyle
{
public:

	// 初始化图标
	static void InitializeIcons();

	// 关闭处理
	static void Shutdown();

private:

	static FName StyleSetName;

	static FName DeleteUnusedAssetsIconName;
	static FName DeleteEmptyFoldersIconName;
	static FName AdvanceDeletionIconName;
	static FName AdvanceDeletionTabIconName;

	static FName LevelEditor_LockSelectionIconName;
	static FName SceneOutliner_SelectionLockIconName;

	// 创建 Slate 样式集
	static TSharedRef<FSlateStyleSet> CreateSlateStyleSet();

	static TSharedPtr<FSlateStyleSet> CreatedSlateStyleSet;

public:

	static FName GetStyleSetName(){ return StyleSetName;};
	
	static FName GetStyleSetDeleteUnusedAssetsIconName(){ return DeleteUnusedAssetsIconName;};
	static FName GetStyleSetDeleteEmptyFoldersIconName(){ return DeleteEmptyFoldersIconName;};
	static FName GetStyleSetAdvanceDeletionIconName(){ return AdvanceDeletionIconName;};
	static FName GetStyleSetAdvanceDeletionTabIconName(){ return AdvanceDeletionTabIconName;};
	
	static FName GetStyleSetLevelEditor_LockSelectionIconName(){ return LevelEditor_LockSelectionIconName;};

	static TSharedPtr<FSlateStyleSet> GetCreatedSlateStyleSet() { return CreatedSlateStyleSet; };
};