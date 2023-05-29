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

#pragma region ProccessDataForAdvanceDeletionTab

	// 删除单个资产
	bool DeleteSingleAssetForAssetList(const FAssetData& AssetDataToDelete);

	// 删除多个资产
	bool DeleteMultipleAssetsForAssetList(const TArray<FAssetData>& AssetsToDelete);

	// 查找未被使用的资产
	void ListUnusedAssetsForAssetList(const TArray<TSharedPtr<FAssetData>>& AssetsDataToFilter, TArray<TSharedPtr<FAssetData>>& OutUnusedAssetsData);

	// 查找所有存在相同名称的资产
	void ListSameNameAssetsForAssetList(const TArray<TSharedPtr<FAssetData>>& AssetsDataToFilter, TArray<TSharedPtr<FAssetData>>& OutSameNameAssetsData);

	// 同步资产到内容浏览器
	void SyncCBToClickedAssetForAssetList(const FString& AssetPathToSync);
	
#pragma endregion 

private:

	FName AdvanceDeletionName = FName("AdvanceDeletion");

// #pragma 代码区域折叠，可快速显示或隐藏某些代码块。
#pragma region ContentBrowserMenuExtention

	// 初始化内容浏览器（CB - ContentBrowser）的菜单扩展
	void InitCBMenuExtention();

	// 自定义内容浏览器菜单扩展器
	TSharedRef<FExtender> CustomCBMenuExtender(const TArray<FString>& SelectedPaths);

	// 扩展"内容浏览器"的文件夹菜单项属性，可根据需要在内添加多个菜单项
	void AddCBMenuEntry(FMenuBuilder& MenuBuilder);
	
	void OnDeleteUnusedAssetButtonClicked();
	
	void OnDeleteEmptyFoldersButtonClicked();

	void OnAdvanceDeletionButtonClicked();

	TArray<FString> FolderPathsSelected;

	// 处理资产重定向问题
	void FixupRedirectors();
	
#pragma endregion

#pragma region CustomEditorTab

	void RegisterAdvanceDeletionTab();

	TSharedRef<SDockTab> OnSpawnAdvanceDeletionTab(const FSpawnTabArgs& SpawnTabArgs);

	// 获取选择的文件夹下的所有资产
	TArray<TSharedPtr<FAssetData>> GetAllAssetDataUnderSelectedFolder();
	
#pragma endregion
	
};
