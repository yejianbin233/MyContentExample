// Copyright Epic Games, Inc. All Rights Reserved.

#include "EditorExtension.h"

#include "ContentBrowserModule.h"
#include "DebugHeader.h"
#include "EditorAssetLibrary.h"
#include "ObjectTools.h"

#define LOCTEXT_NAMESPACE "FEditorExtensionModule"

void FEditorExtensionModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module

	InitCBMenuExtention();
}

void FEditorExtensionModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}


// #pragma 代码区域折叠，可快速显示或隐藏某些代码块。
#pragma region ContentBrowserMenuExtention

void FEditorExtensionModule::InitCBMenuExtention()
{
	// 1. 加载内容浏览器模块
	FContentBrowserModule& ContentBrowserModule = FModuleManager::Get().LoadModuleChecked<FContentBrowserModule>(TEXT("ContentBrowser"));

	// 2. 获取内容浏览器菜单扩展器
	TArray<FContentBrowserMenuExtender_SelectedPaths>& ContentBrowserModuleMenuExtender =
		ContentBrowserModule.GetAllPathViewContextMenuExtenders();
	
	// 3. 创建自定义菜单扩展，该菜单项需要绑定委托，在编辑器点击菜单项时会触发
	FContentBrowserMenuExtender_SelectedPaths CustomCBMenuDelegate;
	CustomCBMenuDelegate.BindRaw(this, &FEditorExtensionModule::CustomCBMenuExtender);

	// 4. 将自定义菜单项添加到内容浏览器菜单扩展器中
	ContentBrowserModuleMenuExtender.Add(CustomCBMenuDelegate);

	// 可将 3、4 步合并在一行
	// ContentBrowserModuleMenuExtender.Add(FContentBrowserMenuExtender_SelectedPaths::CreateRaw(this, &FEditorExtensionModule::CustomCBMenuExtender));

	
}

TSharedRef<FExtender> FEditorExtensionModule::CustomCBMenuExtender(const TArray<FString>& SelectedPaths)
{
	// 使用智能指针来管理 new 出来的对象（管理内存）
	TSharedRef<FExtender> MenuExtender(new FExtender());

	if (SelectedPaths.Num() > 0)
	{
		// 添加菜单扩展
		/*
		 * note：可以在"编辑器设置" -> 搜索"ui Extension"，找到"Display UI Extension Points"后将其勾选
		 * -> 将显示内容浏览器的菜单扩展信息，使用这些信息来扩展菜单，如在指定位置插入新菜单扩展
		 */
		// 在 内容浏览器扩展菜单项"Delete"后插入新菜单项
		// const TSharedPtr<FUICommandList>& CommandList 可定义快捷键，暂时为空项
		//  const FMenuExtensionDelegate& MenuExtensionDelegate 委托，用于设置菜单项的属性
		MenuExtender->AddMenuExtension(FName("Delete"),
			EExtensionHook::After,
			TSharedPtr<FUICommandList>(),
			FMenuExtensionDelegate::CreateRaw(this, &FEditorExtensionModule::AddCBMenuEntry));

		// 设置选择的文件夹路径
		FolderPathsSelected = SelectedPaths;
	}
	return MenuExtender;
}

void FEditorExtensionModule::AddCBMenuEntry(FMenuBuilder& MenuBuilder)
{
	// const FUIAction& UIAction 菜单项触发后将调用的功能
	MenuBuilder.AddMenuEntry(
		FText::FromString(TEXT("Delete Unused Assets")),
		FText::FromString(TEXT("Safely delete all unused assets under folder")),
		FSlateIcon(),
		FExecuteAction::CreateRaw(this, &FEditorExtensionModule::OnDeleteUnusedAssetButtonClicked)
	);
}

void FEditorExtensionModule::OnDeleteUnusedAssetButtonClicked()
{
	if (FolderPathsSelected.Num() > 1)
	{
		DebugHeader::ShowMsgDialog(EAppMsgType::Ok, TEXT("You can only do this to one folder"));
		return;
	}

	const FString FolderPathSelected = FolderPathsSelected[0];
	
	DebugHeader::PrintLog(TEXT("Currently selected folder: ") + FolderPathsSelected[0]);

	// 获取文件夹的资产路径
	TArray<FString> AssetsPathNames = UEditorAssetLibrary::ListAssets(FolderPathSelected);

	if (AssetsPathNames.Num() == 0)
	{
		DebugHeader::ShowMsgDialog(EAppMsgType::Ok, TEXT("No asset found under selected folder"));
		return;
	}

	// 用户决定是否继续进行处理 YesNo
	const EAppReturnType::Type ConfirmResult = DebugHeader::ShowMsgDialog(EAppMsgType::YesNo, TEXT("A total of ") + FString::FromInt(AssetsPathNames.Num()) + TEXT(" found. \nWould you like to procceed? "));

	// 如果用户 No 则不处理。
	if (ConfirmResult == EAppReturnType::No) return;

	TArray<FAssetData> UnusedAssetDatas;
	for (const FString& AssetPathName : AssetsPathNames)
	{
		// Don't touch root folder
		// 筛选不能删除的文件夹，如：Collections、Developers，如果真的删除会导致编辑器崩溃。
		if (AssetPathName.Contains(TEXT("Collections"))
			|| AssetPathName.Contains(TEXT("Developers")))
		{
			DebugHeader::ShowMsgDialog(EAppMsgType::Ok, TEXT("Don't Delete This Folder"));
			continue;
		}

		// 判断文件路径是否存在
		if (UEditorAssetLibrary::DoesAssetExist(AssetPathName)) continue;

		TArray<FString> AssetReferences = UEditorAssetLibrary::FindPackageReferencersForAsset(AssetPathName);

		if (AssetReferences.Num() == 0)
		{
			const FAssetData UnusedAssetData = UEditorAssetLibrary::FindAssetData(AssetPathName);
			UnusedAssetDatas.Add(UnusedAssetData);
		}
	}

	if (UnusedAssetDatas.Num() > 0)
	{
		ObjectTools::DeleteAssets(UnusedAssetDatas);
	}
	else
	{
		DebugHeader::ShowMsgDialog(EAppMsgType::Ok, TEXT("No unused asset found under selected folder"));
	}
}

#pragma endregion 


#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FEditorExtensionModule, EditorExtension)