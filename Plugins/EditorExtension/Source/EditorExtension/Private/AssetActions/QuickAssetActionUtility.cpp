// Fill out your copyright notice in the Description page of Project Settings.


#include "AssetActions/QuickAssetActionUtility.h"
#include "DebugHeader.h"

/* 编辑器扩展功能头文件 */
#include "EditorUtilityLibrary.h" 
#include "EditorAssetLibrary.h"
/* 编辑器扩展功能头文件 */

#include "Misc/MessageDialog.h" // 信息对话框

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
