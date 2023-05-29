// Fill out your copyright notice in the Description page of Project Settings.


#include "EditorExtensionStyle.h"
#include "Interfaces/IPluginManager.h"
#include "Styling/SlateStyleRegistry.h"

// 静态变量需要在 Cpp 中初始化
FName FEditorExtensionStyle::StyleSetName = FName("EditorExtensionStyle");
FName FEditorExtensionStyle::DeleteUnusedAssetsIconName = FName("ContentBrowser.DeleteUnusedAssetsIcon");
FName FEditorExtensionStyle::DeleteEmptyFoldersIconName = FName("ContentBrowser.DeleteEmptyFoldersIcon");
FName FEditorExtensionStyle::AdvanceDeletionIconName = FName("ContentBrowser.AdvanceDeletionIcon");
FName FEditorExtensionStyle::AdvanceDeletionTabIconName = FName("ContentBrowser.AdvanceDeletionTabIcon");

TSharedPtr<FSlateStyleSet> FEditorExtensionStyle::CreatedSlateStyleSet = nullptr;

void FEditorExtensionStyle::InitializeIcons()
{
	if (!CreatedSlateStyleSet.IsValid())
	{
		CreatedSlateStyleSet = CreateSlateStyleSet();
		FSlateStyleRegistry::RegisterSlateStyle(*CreatedSlateStyleSet);

	}
}

void FEditorExtensionStyle::Shutdown()
{
	if (CreatedSlateStyleSet.IsValid())
	{
		// 当关闭时，取消注册
		FSlateStyleRegistry::UnRegisterSlateStyle(*CreatedSlateStyleSet);
		CreatedSlateStyleSet.Reset();
	}
}

TSharedRef<FSlateStyleSet> FEditorExtensionStyle::CreateSlateStyleSet()
{
	TSharedRef<FSlateStyleSet> CustomStyleSet = MakeShareable(new FSlateStyleSet(StyleSetName));

	// 插件管理器，获取插件的基础路径
	const FString IconDirectory = IPluginManager::Get().FindPlugin(TEXT("EditorExtension"))->GetBaseDir() /"Resources";

	// 设置样式集的文件根路径
	CustomStyleSet->SetContentRoot(IconDirectory);

	// 添加数据
	const FVector2D Icon16x16(16, 16);

	CustomStyleSet->Set(AdvanceDeletionTabIconName,
		new FSlateImageBrush(IconDirectory/"AdvanceDeletionTabIcon.png", Icon16x16));
	
	CustomStyleSet->Set(DeleteUnusedAssetsIconName,
		new FSlateImageBrush(IconDirectory/"DeleteUnusedAssetIcon.png", Icon16x16));

	CustomStyleSet->Set(DeleteEmptyFoldersIconName,
		new FSlateImageBrush(IconDirectory/"DeleteEmptyFolderIcon.png", Icon16x16));

	CustomStyleSet->Set(AdvanceDeletionIconName,
		new FSlateImageBrush(IconDirectory/"AdvanceDeletionIcon.png", Icon16x16));
	
	return CustomStyleSet;
}
