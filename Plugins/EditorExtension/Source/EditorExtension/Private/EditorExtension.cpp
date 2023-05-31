// Copyright Epic Games, Inc. All Rights Reserved.

#include "EditorExtension.h"

#include "AssetToolsModule.h"
#include "ContentBrowserModule.h"
#include "DebugHeader.h"
#include "EditorAssetLibrary.h"
#include "EditorExtensionUICommands.h"
#include "LevelEditor.h"
#include "ObjectTools.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "SlateWidgets/AdvanceDeletionWidget.h"
#include "EditorExtension/CustomStyle/EditorExtensionStyle.h"
#include "Engine/Selection.h"
#include "Subsystems/EditorActorSubsystem.h"

#define LOCTEXT_NAMESPACE "FEditorExtensionModule"


bool FEditorExtensionModule::GetEditorActorSubsystem()
{
	if (!WeakEditorActorSubsystem.IsValid())
	{
		WeakEditorActorSubsystem = GEditor->GetEditorSubsystem<UEditorActorSubsystem>();
	}

	return WeakEditorActorSubsystem.IsValid();
}

void FEditorExtensionModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module

	// 初始化 Icons
	FEditorExtensionStyle::InitializeIcons();

	
	InitCBMenuExtention();
	
	// 启用该插件时会注册内容浏览器文件夹菜单项
	RegisterAdvanceDeletionTab();

	// 模块启动时注册快捷键，可以在"编辑器偏好设置 -> 快捷键中查看"
	FEditorExtensionUICommands::Register();

	// 初始化构造快捷键列表，等到 InitLevelEditorExtension 处再顺便将快捷键列表添加
	InitCustomUICommands();
	
	// 启用关卡场景中 Actor 的右键编辑菜单
	InitLevelEditorExtension();

	// 初始化自定义选择事件（当在场景中选择 Actor 时触发）
	InitCustomSelectionEvent();
}

void FEditorExtensionModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	// 取消启用该插件时会取消注册内容浏览器文件夹菜单项
	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(AdvanceDeletionName);

	FEditorExtensionStyle::Shutdown();

	// 当插件关闭后，将快捷键功能取消注册
	FEditorExtensionUICommands::Unregister();
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
		FSlateIcon(FEditorExtensionStyle::GetStyleSetName(), FEditorExtensionStyle::GetStyleSetDeleteUnusedAssetsIconName()), // 图标
		FExecuteAction::CreateRaw(this, &FEditorExtensionModule::OnDeleteUnusedAssetButtonClicked)
	);

	MenuBuilder.AddMenuEntry(
		FText::FromString(TEXT("Delete Empty Folders")),
		FText::FromString(TEXT("Safely delete all emptry folder")),
		FSlateIcon(FEditorExtensionStyle::GetStyleSetName(), FEditorExtensionStyle::GetStyleSetDeleteEmptyFoldersIconName()), // 图标
		FExecuteAction::CreateRaw(this, &FEditorExtensionModule::OnDeleteEmptyFoldersButtonClicked)
	);

	MenuBuilder.AddMenuEntry(
		FText::FromString(TEXT("Advance Deletion")),
		FText::FromString(TEXT("Advance Deletion Tab")),
		FSlateIcon(FEditorExtensionStyle::GetStyleSetName(), FEditorExtensionStyle::GetStyleSetAdvanceDeletionIconName()), // 图标
		FExecuteAction::CreateRaw(this, &FEditorExtensionModule::OnAdvanceDeletionButtonClicked)
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
		DebugHeader::ShowMsgDialog(EAppMsgType::Ok, TEXT("No asset found under selected folder"), false);
		return;
	}

	// 用户决定是否继续进行处理 YesNo
	const EAppReturnType::Type ConfirmResult = DebugHeader::ShowMsgDialog(EAppMsgType::YesNo, TEXT("A total of ") + FString::FromInt(AssetsPathNames.Num()) + TEXT(" found. \nWould you like to procceed? "));

	// 如果用户 No 则不处理。
	if (ConfirmResult == EAppReturnType::No) return;

	// 解决资产重定向
	FixupRedirectors();
	
	TArray<FAssetData> UnusedAssetDatas;
	for (const FString& AssetPathName : AssetsPathNames)
	{
		// Don't touch root folder
		// 筛选不能删除的文件夹，如：Collections、Developers，如果真的删除会导致编辑器崩溃。
		if (AssetPathName.Contains(TEXT("Collections"))
			|| AssetPathName.Contains(TEXT("Developers"))
			|| AssetPathName.Contains(TEXT("__ExternalActors__"))
			|| AssetPathName.Contains(TEXT("__ExternalObjects__")))
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
		DebugHeader::ShowMsgDialog(EAppMsgType::Ok, TEXT("No unused asset found under selected folder"), false);
	}
}

void FEditorExtensionModule::OnDeleteEmptyFoldersButtonClicked()
{
	FixupRedirectors();

	const FString& FolderPathSelected = FolderPathsSelected[0];
	
	TArray<FString> FolderPathsArrays = UEditorAssetLibrary::ListAssets(FolderPathSelected, true, true);

	uint32 Counter = 0;

	FString EmptyFolderPathsNames;
	TArray<FString> EmptyFoldersPathsArray;

	for (const FString& FolderPath : FolderPathsArrays)
	{
		if (FolderPath.Contains(TEXT("Collections"))
			|| FolderPath.Contains(TEXT("Developers"))
			|| FolderPath.Contains(TEXT("__ExternalActors__"))
			|| FolderPath.Contains(TEXT("__ExternalObjects__")))
		{
			DebugHeader::ShowMsgDialog(EAppMsgType::Ok, TEXT("Don't Delete This Folder"));
			continue;
		}

		// 判断是否是文件夹
		if (!UEditorAssetLibrary::DoesDirectoryExist(FolderPath)) continue;

		// 判断文件夹下是否有资产
		if (UEditorAssetLibrary::DoesDirectoryHaveAssets(FolderPath)) continue;

		EmptyFolderPathsNames.Append(FolderPath);
		EmptyFolderPathsNames.Append(TEXT("\n"));

		EmptyFoldersPathsArray.Add(FolderPath);
	}

	if (EmptyFoldersPathsArray.Num() == 0)
	{
		DebugHeader::ShowMsgDialog(EAppMsgType::Ok, TEXT("No empty folder found under selected folder"), false);

		return;
	}

	 EAppReturnType::Type ConfirmResult = DebugHeader::ShowMsgDialog(EAppMsgType::OkCancel, TEXT("Empty folders found in \n") + EmptyFolderPathsNames + TEXT("\nWould you like to delete all?"), false);

	if (ConfirmResult == EAppReturnType::Cancel) return;

	for (const FString& EmptyFolderPath : EmptyFoldersPathsArray)
	{
		bool DeleteResult = UEditorAssetLibrary::DeleteDirectory(EmptyFolderPath);

		DeleteResult ? ++Counter : DebugHeader::PrintLog(TEXT("Failed to delete ") + EmptyFolderPath);
	}

	if (Counter > 0)
	{
		DebugHeader::ShowNotifyInfo(TEXT("Successfully deleted ") + FString::FromInt(Counter) + TEXT(" folders"));
	}
	
}

void FEditorExtensionModule::OnAdvanceDeletionButtonClicked()
{
	// 解决文件重定向问题
	FixupRedirectors();
	// 尝试唤醒 Tab 菜单，需要与调用 RegisterNomadTabSpawner 传入的 FName 一致（const FName TabId，类似函数的使用方式类似）。
	FGlobalTabmanager::Get()->TryInvokeTab(AdvanceDeletionName);
} 

void FEditorExtensionModule::FixupRedirectors()
{
	TArray<UObjectRedirector*> RedirectorsToFixArray;

	// 加载指定模块
	FAssetRegistryModule& AssetRegistryModule = FModuleManager::Get().LoadModuleChecked<FAssetRegistryModule>(TEXT("AssetRegistry"));

	FARFilter Filter;
	Filter.bRecursivePaths = true; // 允许递归
	Filter.PackagePaths.Emplace("/Game"); // 检索 /Game 文件夹下的资产
	Filter.ClassPaths.Emplace("ObjectRedirector"); // 只检索指定类型（ObjectRedirector）
	
	TArray<FAssetData> OutRedirectors;
	AssetRegistryModule.Get().GetAssets(Filter, OutRedirectors);

	for (const FAssetData& RedirectorData : OutRedirectors)
	{
		if (UObjectRedirector* RedirectorToFix = Cast<UObjectRedirector>(RedirectorData.GetAsset()))
		{
			RedirectorsToFixArray.Add(RedirectorToFix);
		}
	}

	FAssetToolsModule& AssetToolsModule = FModuleManager::LoadModuleChecked<FAssetToolsModule>(TEXT("AssetTools"));

	// 解决资产重定向问题
	AssetToolsModule.Get().FixupReferencers(RedirectorsToFixArray);
}



#pragma endregion

#pragma region CustomEditorTab

void FEditorExtensionModule::RegisterAdvanceDeletionTab()
{
	FTabSpawnerEntry& TabSpawnerEntry = FGlobalTabmanager::Get()->RegisterNomadTabSpawner(AdvanceDeletionName
		, FOnSpawnTab::CreateRaw(this, &FEditorExtensionModule::OnSpawnAdvanceDeletionTab))
		.SetDisplayName(FText::FromString(AdvanceDeletionName.ToString()))
		.SetIcon(FSlateIcon(FEditorExtensionStyle::GetStyleSetName(), FEditorExtensionStyle::GetStyleSetAdvanceDeletionTabIconName()));
}

TSharedRef<SDockTab> FEditorExtensionModule::OnSpawnAdvanceDeletionTab(const FSpawnTabArgs& SpawnTabArgs)
{
	const FString& FolderPath = FolderPathsSelected[0];
	return SNew(SDockTab).TabRole(ETabRole::NomadTab)
	[
		SNew(SAdvanceDeletionTag)
			.AssetsDataToStore(GetAllAssetDataUnderSelectedFolder())
			.CurrentSelectedFolder(FolderPath)
	];
}

TArray<TSharedPtr<FAssetData>> FEditorExtensionModule::GetAllAssetDataUnderSelectedFolder()
{
	TArray<TSharedPtr<FAssetData>> AvaiableAssetDatas;

	if (FolderPathsSelected.Num() == 0)
	{
		return AvaiableAssetDatas;
	}
	
	const FString SelectPathSelected = FolderPathsSelected[0];
	
	TArray<FString> AssetPathNames = UEditorAssetLibrary::ListAssets(SelectPathSelected);

	for (const FString& AssetPathName : AssetPathNames)
	{
		// Don't touch root folder
		// 筛选不能删除的文件夹，如：Collections、Developers，如果真的删除会导致编辑器崩溃。
		if (AssetPathName.Contains(TEXT("Collections"))
			|| AssetPathName.Contains(TEXT("Developers"))
			|| AssetPathName.Contains(TEXT("__ExternalActors__"))
			|| AssetPathName.Contains(TEXT("__ExternalObjects__")))
		{
			DebugHeader::ShowMsgDialog(EAppMsgType::Ok, TEXT("Don't Delete This Folder"));
			continue;
		}

		if (!UEditorAssetLibrary::DoesAssetExist(AssetPathName)) continue;

		const FAssetData AssetData = UEditorAssetLibrary::FindAssetData(AssetPathName);

		AvaiableAssetDatas.Add(MakeShared<FAssetData>(AssetData));
	}

	return AvaiableAssetDatas;
}

#pragma endregion

#pragma region ProccessDataForAdvanceDeletionTab

bool FEditorExtensionModule::DeleteSingleAssetForAssetList(const FAssetData& AssetDataToDelete)
{
	TArray<FAssetData> AssetDatasForDeletion;
	AssetDatasForDeletion.Add(AssetDataToDelete);
	
	if (ObjectTools::DeleteAssets(AssetDatasForDeletion) > 0)
	{
		return true;
	}
	
	return false;
}

bool FEditorExtensionModule::DeleteMultipleAssetsForAssetList(const TArray<FAssetData>& AssetsToDelete)
{
	if (ObjectTools::DeleteAssets(AssetsToDelete) > 0)
	{
		return true;
	}

	return false;
}

void FEditorExtensionModule::ListUnusedAssetsForAssetList(const TArray<TSharedPtr<FAssetData>>& AssetsDataToFilter,
	TArray<TSharedPtr<FAssetData>>& OutUnusedAssetsData)
{
	OutUnusedAssetsData.Empty();
	
	for (const TSharedPtr<FAssetData>& DataSharedPtr : AssetsDataToFilter)
	{
		TArray<FString> AssetReferencers = UEditorAssetLibrary::FindPackageReferencersForAsset(DataSharedPtr->GetObjectPathString());

		// 如果资产未被引用，则将其添加到 OutUnusedAssetsData
		if (AssetReferencers.Num() == 0)
		{
			OutUnusedAssetsData.Add(DataSharedPtr);
		}
	}
}

void FEditorExtensionModule::ListSameNameAssetsForAssetList(const TArray<TSharedPtr<FAssetData>>& AssetsDataToFilter,
	TArray<TSharedPtr<FAssetData>>& OutSameNameAssetsData)
{
	OutSameNameAssetsData.Empty();

	TMultiMap<FString, TSharedPtr<FAssetData>> AssetsInfoMultiMap;

	for (const TSharedPtr<FAssetData>& DataSharedPtr : AssetsDataToFilter)
	{
		AssetsInfoMultiMap.Emplace(DataSharedPtr->AssetName.ToString(), DataSharedPtr);
	}

	for (const TSharedPtr<FAssetData>& DataSharedPtr : AssetsDataToFilter)
	{
		TArray<TSharedPtr<FAssetData>> OutAssetsData;
		AssetsInfoMultiMap.MultiFind(DataSharedPtr->AssetName.ToString(), OutAssetsData);

		if (OutAssetsData.Num() <= 1)
		{
			continue;
		}

		for (TSharedPtr<FAssetData> SameNameAssetData : OutAssetsData)
		{
			if (SameNameAssetData.IsValid())
			{
				OutSameNameAssetsData.AddUnique(SameNameAssetData);
			}
		}
	}
}

void FEditorExtensionModule::SyncCBToClickedAssetForAssetList(const FString& AssetPathToSync)
{
	TArray<FString> AssetsPathSync;
	AssetsPathSync.Add(AssetPathToSync);

	UEditorAssetLibrary::SyncBrowserToObjects(AssetsPathSync);
}

#pragma endregion 

#pragma region LevelEditorMenuExtension

void FEditorExtensionModule::InitLevelEditorExtension()
{
	FLevelEditorModule& LevelEditorModule = FModuleManager::Get().LoadModuleChecked<FLevelEditorModule>(TEXT("LevelEditor"));

	// 获取快捷键列表，并将新自定义快捷键列表添加进去
	TSharedRef<FUICommandList> ExistingLevelCommands = LevelEditorModule.GetGlobalLevelEditorActions();
	ExistingLevelCommands->Append(CustomUICommands.ToSharedRef());
	
	// 获取关卡视口菜单扩展器
	TArray<FLevelEditorModule::FLevelViewportMenuExtender_SelectedActors>& LevelEditorMenuExtenders =
		LevelEditorModule.GetAllLevelViewportContextMenuExtenders();

	// 添加 Actor 的菜单项
	LevelEditorMenuExtenders.Add(FLevelEditorModule::FLevelViewportMenuExtender_SelectedActors::CreateRaw(this, &FEditorExtensionModule::CustomLevelEditorMenuExtender));
}

TSharedRef<FExtender> FEditorExtensionModule::CustomLevelEditorMenuExtender(
	const TSharedRef<FUICommandList> UICommandList, const TArray<AActor*> SelectedActors)
{
	TSharedRef<FExtender> MenuExtender = MakeShareable(new FExtender());

	if (SelectedActors.Num() > 0)
	{
		MenuExtender->AddMenuExtension(
			FName("ActorOptions"),
			EExtensionHook::Before,
			UICommandList,
			FMenuExtensionDelegate::CreateRaw(this, &FEditorExtensionModule::AddLevelEditorMenuEntry)
		);
	}
	
	return MenuExtender;
}

void FEditorExtensionModule::AddLevelEditorMenuEntry(FMenuBuilder& MenuBuilder)
{
	MenuBuilder.AddMenuEntry(
		FText::FromString(TEXT("Lock Actor Selection")),
		FText::FromString(TEXT("Prevent actor from being selected.")),
		FSlateIcon(),
		FExecuteAction::CreateRaw(this, &FEditorExtensionModule::OnLockActorSelectionButtonClick)
	);

	MenuBuilder.AddMenuEntry(
		FText::FromString(TEXT("Unlock all Actor Selection")),
		FText::FromString(TEXT("Remove the selection constraint on all actor.")),
		FSlateIcon(),
		FExecuteAction::CreateRaw(this, &FEditorExtensionModule::OnUnlockActorSelectionButtonClick)
	);
}

void FEditorExtensionModule::OnLockActorSelectionButtonClick()
{
	if (!GetEditorActorSubsystem()) return;

	TArray<AActor*> SelectedActors = WeakEditorActorSubsystem->GetSelectedLevelActors();

	if (SelectedActors.Num() == 0)
	{
		DebugHeader::ShowNotifyInfo(TEXT("No actor selected."));
	}
	
	for (AActor* SelectedActor : SelectedActors)
	{
		if (!SelectedActor) continue;

		if (CheckIsActorSelectionLocked(SelectedActor)) continue;
		
		LockActorSelection(SelectedActor);
	}
}

void FEditorExtensionModule::OnUnlockActorSelectionButtonClick()
{
	if (!GetEditorActorSubsystem()) return;

	TArray<AActor*> SelectedActors = WeakEditorActorSubsystem->GetSelectedLevelActors();

	if (SelectedActors.Num() == 0)
	{
		DebugHeader::ShowNotifyInfo(TEXT("No actor selected."));
	}
	
	for (AActor* SelectedActor : SelectedActors)
	{
		if (!SelectedActor) continue;

		if (!CheckIsActorSelectionLocked(SelectedActor)) continue;
		
		UnlockActorSelection(SelectedActor);
	}
}

#pragma endregion

#pragma region SelectionLock

void FEditorExtensionModule::InitCustomSelectionEvent()
{
	USelection* UserSelection = GEditor->GetSelectedActors();
	
	// 每当在视口中选中 Actor 时触发事件
	UserSelection->SelectObjectEvent.AddRaw(this, &FEditorExtensionModule::OnActorSelected);
}

void FEditorExtensionModule::OnActorSelected(UObject* SelectedObject)
{
	if (!GetEditorActorSubsystem()) return ;
	
	if (AActor* SelectedActor = Cast<AActor>(SelectedObject))
	{
		if (CheckIsActorSelectionLocked(SelectedActor))
		{
			// 不选择 Actor
			WeakEditorActorSubsystem->SetActorSelectionState(SelectedActor, false);
		}
	}
}

void FEditorExtensionModule::LockActorSelection(AActor* ActorToProcess)
{
	if (!ActorToProcess) return ;

	if (!ActorToProcess->ActorHasTag(ActorLockTagName))
	{
		ActorToProcess->Tags.Add(ActorLockTagName);
	}
	
}

void FEditorExtensionModule::UnlockActorSelection(AActor* ActorToProcess)
{
	if (!ActorToProcess) return ;

	if (ActorToProcess->ActorHasTag(ActorLockTagName))
	{
		ActorToProcess->Tags.Remove(ActorLockTagName);
	}
}

bool FEditorExtensionModule::CheckIsActorSelectionLocked(AActor* ActorToProcess)
{
	// 通过检查 Actor 是否具有标签来判断是否被选择锁定
	if (!ActorToProcess)
	{
		return false;
	}
	return ActorToProcess->ActorHasTag(ActorLockTagName);
}

#pragma endregion

#pragma region CustomEditorUICommands

void FEditorExtensionModule::InitCustomUICommands()
{
	CustomUICommands = MakeShareable(new FUICommandList());

	FEditorExtensionUICommands ExtensionUICommands = FEditorExtensionUICommands::Get();
	
	// 绑定快捷键与快捷键功能
	CustomUICommands->MapAction(
		FEditorExtensionUICommands::Get().LockActorsSelection,
		FExecuteAction::CreateRaw(this, &FEditorExtensionModule::OnSelectionLockHotkeyPressed)
	);

	CustomUICommands->MapAction(
		FEditorExtensionUICommands::Get().UnlockActorsSelection,
		FExecuteAction::CreateRaw(this, &FEditorExtensionModule::OnSelectionUnlockHotkeyPressed)
	);
}

void FEditorExtensionModule::OnSelectionLockHotkeyPressed()
{
	OnLockActorSelectionButtonClick();
}

void FEditorExtensionModule::OnSelectionUnlockHotkeyPressed()
{
	OnUnlockActorSelectionButtonClick();
}

#pragma endregion 

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FEditorExtensionModule, EditorExtension)