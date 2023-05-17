// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FEditorExtensionModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

private:

// #pragma 代码区域折叠，可快速显示或隐藏某些代码块。
#pragma region ContentBrowserMenuExtention

	// 初始化内容浏览器（CB - ContentBrowser）的菜单扩展
	void InitCBMenuExtention();

	// 自定义内容浏览器菜单扩展器
	TSharedRef<FExtender> CustomCBMenuExtender(const TArray<FString>& SelectedPaths);

	// 扩展菜单项属性
	void AddCBMenuEntry(FMenuBuilder& MenuBuilder);
	
	void OnDeleteUnusedAssetButtonClicked();

	TArray<FString> FolderPathsSelected;
	
#pragma endregion 
	
};
