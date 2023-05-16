// Fill out your copyright notice in the Description page of Project Settings.


#include "AssetActions/QuickAssetActionUtility.h"
#include "DebugHeader.h"

/* 编辑器扩展功能头文件 */
#include "EditorUtilityLibrary.h" 
#include "EditorAssetLibrary.h"
/* 编辑器扩展功能头文件 */

#include "Misc/MessageDialog.h" // 信息对话框
#include "ObjectTools.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "AssetToolsModule.h"

void UQuickAssetActionUtility::DuplicateAssets(int32 NumOfDuplicates)
{
	if (NumOfDuplicates <= 0)
	{

		// 使用弹窗来显示提示信息。
		ShowMsgDialog(EAppMsgType::Ok, TEXT("Please Enter A Valid Number!"));
		return;
	}

	// 获取在编辑器中当前所选择的资产的资产数据。
	TArray<FAssetData> SelectedAssetDatas = UEditorUtilityLibrary::GetSelectedAssetData();
	uint32 Counter = 0;

	for(const FAssetData& SelectedAssetData : SelectedAssetDatas)
	{
		for (int32 i = 0; i < NumOfDuplicates; ++i)
		{
			const FString SourceAssetPath = SelectedAssetData.GetObjectPathString(); // 源资产（被选中的资产）的对象资产路径（包路径 + 资产名，如 /Game/Blueprint/BP_NewBlueprint.BP_NewBlueprint，其中 包路径为"/Game/Blueprint/"，资产名为“BP_NewBlueprint.BP_NewBlueprint”）

			const FString NewDuplicatedAssetName = SelectedAssetData.AssetName.ToString() + TEXT("_") + FString::FromInt(i); // 获取资产名称，然后再用于创建新资产的名称

			const FString NewPathName = FPaths::Combine(SelectedAssetData.PackagePath.ToString(), NewDuplicatedAssetName); // 获取资产的包路径，再与新的资产名称组合成新的资产路径

			if (UEditorAssetLibrary::DuplicateAsset(SourceAssetPath, NewPathName)) // 复制资产
			{
				UEditorAssetLibrary::SaveAsset(NewPathName, false); // 保存资产，并将 bOnlylfsDirty 设置为 false。
				++Counter;
			} 
		}
	}

	if (Counter > 0)
	{
		// PrintLog(TEXT("Successfully Duplicated " + FString::FromInt(Counter) + " Files"));

		ShowNotifyInfo(TEXT("Successfully Duplicated " + FString::FromInt(Counter) + " Files"));
	}
}

void UQuickAssetActionUtility::AddPrefixes()
{
	TArray<UObject*> SelectedObjects = UEditorUtilityLibrary::GetSelectedAssets();
	uint32 Counter = 0;

	for (UObject* SelectedObject : SelectedObjects)
	{
		if (!SelectedObject)	continue;

		const FString* PrefixFound = PrefixMap.Find(SelectedObject->GetClass());

		if (!PrefixFound || PrefixFound->IsEmpty())
		{
			PrintLog(TEXT("Failed to find prefix for class") + SelectedObject->GetClass()->GetName());
			continue;
		}

		FString OldName = SelectedObject->GetName();

		// 检查是否已经有正确的前缀，如果有则忽略
		if (OldName.StartsWith(*PrefixFound))
		{
			PrintLog(OldName + TEXT(" already has prefix added"));
			continue;
		}
		
		if(SelectedObject->IsA<UMaterialInstanceConstant>())
		{
			if (OldName.StartsWith("M_", ESearchCase::IgnoreCase))
			{
				OldName.RemoveFromStart(TEXT("M_"), ESearchCase::IgnoreCase);
			}

			if (OldName.EndsWith("_Inst", ESearchCase::IgnoreCase))
			{
				OldName.RemoveFromEnd(TEXT("_Inst"), ESearchCase::IgnoreCase);
			}
		}

		// 添加前缀
		const FString NewNameWithPrefix = *PrefixFound + OldName;
		// 重命名资产
		UEditorUtilityLibrary::RenameAsset(SelectedObject, NewNameWithPrefix);
		++Counter;
	}

	if (Counter > 0)
	{
		ShowNotifyInfo(TEXT("Successfully renamed ") + FString::FromInt(Counter));
	}
}

void UQuickAssetActionUtility::RemoveUnusedAssets()
{
	TArray<FAssetData> SelectedAssetsData = UEditorUtilityLibrary::GetSelectedAssetData();

	TArray<FAssetData> UnusedAssetDatas;

	// 先执行解决资产的重定向问题的流程
	FixupRedirectors();

	for (const FAssetData& SelectedAssetData : SelectedAssetsData)
	{
		// 查找资产的包引用
		TArray<FString> AssetReferences = UEditorAssetLibrary::FindPackageReferencersForAsset(SelectedAssetData.GetObjectPathString());
		
		if (AssetReferences.Num() == 0)
		{
			// 如果包引用数量为 0，则说明没有被使用。
			UnusedAssetDatas.Add(SelectedAssetData);
		}
	}

	if (UnusedAssetDatas.Num() == 0)
	{
		ShowMsgDialog(EAppMsgType::Ok, TEXT("No unused asset found among selected assets"), false);
		return;
	}

	// 删除资产
	const int32 NumOfAssetsDeleted = ObjectTools::DeleteAssets(UnusedAssetDatas);
	
	if (NumOfAssetsDeleted == 0) return;

	ShowNotifyInfo(TEXT("Successfully deleted " + FString::FromInt(NumOfAssetsDeleted) + " unused assets"));
}

void UQuickAssetActionUtility::FixupRedirectors()
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
