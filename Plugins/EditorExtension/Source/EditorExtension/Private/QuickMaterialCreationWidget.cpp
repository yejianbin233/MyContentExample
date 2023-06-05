// Fill out your copyright notice in the Description page of Project Settings.


#include "QuickMaterialCreationWidget.h"
#include "DebugHeader.h"
#include "EditorUtilityLibrary.h"
#include "EditorAssetLibrary.h"
#include "AssetToolsModule.h"
#include "Factories/MaterialFactoryNew.h"
#include "Materials/MaterialExpressionTextureSample.h"
#include "Materials/MaterialInstanceConstant.h"
#include "Factories/MaterialInstanceConstantFactoryNew.h"

#pragma region QuickMaterialCreationCore

void UQuickMaterialCreationWidget::CreateMaterialFromSelectedTextures()
{
	// 是否自定义材质名称
	if (bCustomMaterialName)
	{
		// 如果自定义材质名称，则不允许名称为空或保持默认名称。
		if (MaterialName.IsEmpty() || MaterialName.Equals(TEXT("M_")))
		{
			DebugHeader::ShowMsgDialog(EAppMsgType::Ok, TEXT("Please enter a valid name."));
			return;
		}
	}

	// 通过 UEditorUtilityLibrary 来获取在"内容浏览器"中选择的资产数据，需要注意的时应该判断使用纹理资产。
	TArray<FAssetData> SelectedAssetsData = UEditorUtilityLibrary::GetSelectedAssetData();

	TArray<UTexture2D*> SelectedTexturesArray;

	FString SeletedTextureFolderPath;

	uint32 PinsConnectedCounter = 0;

	// 处理所选择的数据，筛选所需的数据
	bool ProcessResult = ProcessSelectedData(SelectedAssetsData, SelectedTexturesArray, SeletedTextureFolderPath);

	if (!ProcessResult)
	{
		MaterialName = MaterialDefaultPrefixName;
		return ;
	}

	// 检查名称是否已经被使用
	if (CheckIsNameUsed(SeletedTextureFolderPath, MaterialName))
	{
		MaterialName = MaterialDefaultPrefixName;
		
		return ;
	};

	// 创建"材质"资产
	UMaterial* CreatedMaterial = CreateMaterialAsset(MaterialName, SeletedTextureFolderPath);

	if (!CreatedMaterial)
	{
		DebugHeader::ShowMsgDialog(EAppMsgType::Ok, TEXT("Fail to create material."));
		return;
	}
	
	for (UTexture2D* SelectedTexture : SelectedTexturesArray)
	{
		if (!SelectedTexture) continue;

		// 通过手动指定的方式，来选择纹理在材质图表中连接节点的方式
		switch (ChannelPackingType)
		{
			
			case E_ChannelPackingType::ECPT_NoChannelPacking:
				DefaultCreateMaterialNodes(CreatedMaterial, SelectedTexture, PinsConnectedCounter);
				break;
			
			case E_ChannelPackingType::ECPT_ORM:
				// 对单一纹理的不同通道数据进行处理
				ORM_CreateMaterialNodes(CreatedMaterial, SelectedTexture, PinsConnectedCounter);
				break;
			
			case E_ChannelPackingType::ECPT_MAX:
				break;
			
			default: ;
		}
	}

	if (PinsConnectedCounter > 0)
	{
		DebugHeader::ShowNotifyInfo(TEXT("Successfully connected ") + FString::FromInt(PinsConnectedCounter) + TEXT(" pins"));
	}

	// 是否定义材质实例
	if (bCreateMaterialInstance)
	{
		CreateMaterialInstanceAsset(CreatedMaterial, MaterialName, SeletedTextureFolderPath);
	}

	MaterialName = MaterialDefaultPrefixName;
}
#pragma endregion 


#pragma region QuickMaterialCreation

void UQuickMaterialCreationWidget::ORM_CreateMaterialNodes(UMaterial* CreatedMaterial, UTexture2D* SelectedTexture,
	uint32& PinsConnnectedCounter)
{
	UMaterialExpressionTextureSample* TextureSampleNode = NewObject<UMaterialExpressionTextureSample>(CreatedMaterial);

	if (!TextureSampleNode) return;

	TryConnectedORM(TextureSampleNode, SelectedTexture, CreatedMaterial, PinsConnnectedCounter);
}

bool UQuickMaterialCreationWidget::ProcessSelectedData(const TArray<FAssetData>& SelectedDataToProcess,
                                                       TArray<UTexture2D*>& OutSelectedTexturesArray, FString& OutSelectedTexturePackagePath)
{
	if (SelectedDataToProcess.Num() == 0)
	{
		DebugHeader::ShowMsgDialog(EAppMsgType::Ok, TEXT("No Texture selected"));
		return false;
	}

	bool bMaterialNameSet = false;

	// 遍历资产，筛选资产
	for (const FAssetData& SelectedData : SelectedDataToProcess)
	{
		UObject* SelectedAsset = SelectedData.GetAsset();
		
		if (!SelectedAsset) continue;

		// 转换为 2D 纹理
		UTexture2D* SelectedTexture = Cast<UTexture2D>(SelectedAsset);

		if (!SelectedTexture)
		{
			DebugHeader::ShowMsgDialog(EAppMsgType::Ok, TEXT("Please select only textures \n") + SelectedAsset->GetName() + TEXT(" is not a texture."));
			return false;
		};

		OutSelectedTexturesArray.Add(SelectedTexture);

		if (OutSelectedTexturePackagePath.IsEmpty())
		{
			OutSelectedTexturePackagePath = SelectedData.PackagePath.ToString();
		}

		if (!bCustomMaterialName && !bMaterialNameSet)
		{
			// 如果自定义名称，将使用资产的名称作为创建后材质的名称
			MaterialName = SelectedAsset->GetName();
			// 清除纹理默认的前缀
			MaterialName.RemoveFromStart(TEXT("T_"));
			// 为材质添加前缀
			MaterialName.InsertAt(0, TEXT("M_"));

			bMaterialNameSet = true;
		}
	}
	
	return true;
}

bool UQuickMaterialCreationWidget::CheckIsNameUsed(const FString& FolderPathToCheck, const FString& MaterialNameToCheck)
{
	TArray<FString> ExistingAssetsPaths = UEditorAssetLibrary::ListAssets(FolderPathToCheck, false);

	for (const FString& ExistingAssetPath : ExistingAssetsPaths)
	{
		const FString ExistingAssetName = FPaths::GetBaseFilename(ExistingAssetPath);
		
		if (ExistingAssetName.Equals(MaterialNameToCheck))
		{
			DebugHeader::ShowMsgDialog(EAppMsgType::Ok, MaterialNameToCheck + TEXT(" is already used by asset."), true);
			return true;
		}
	}

	return false;
}

UMaterial* UQuickMaterialCreationWidget::CreateMaterialAsset(const FString& NameOfTheMaterial,
	const FString& PathToPutMaterial)
{
	// 使用 "FAssetToolsModule" 模块
	FAssetToolsModule& AssetToolsModule = FModuleManager::LoadModuleChecked<FAssetToolsModule>(TEXT("AssetTools"));

	// 使用 "UMaterialFactoryNew" 工厂类来创建材质资产
	UMaterialFactoryNew* MaterialFactoryNew = NewObject<UMaterialFactoryNew>();

	// 创建材质
	UObject* CreatedObject = AssetToolsModule.Get().CreateAsset(NameOfTheMaterial, // 资产名称
		PathToPutMaterial, // 资产路径
		UMaterial::StaticClass(), // 资产类型
		MaterialFactoryNew);

	return Cast<UMaterial>(CreatedObject);
}

void UQuickMaterialCreationWidget::DefaultCreateMaterialNodes(UMaterial* CreateMaterial, UTexture2D* SelectedTexture,
	uint32& PinsConnnectedCounter)
{
	// 创建在材质图表中所使用的"纹理采样"节点
	UMaterialExpressionTextureSample* TextureSampleNode = NewObject<UMaterialExpressionTextureSample>(CreateMaterial);

	if (!TextureSampleNode) return;

	// 基础颜色节点是否已连接
	if (!CreateMaterial->HasBaseColorConnected())
	{
		if (TryConnectedBaseColor(TextureSampleNode, SelectedTexture, CreateMaterial))
		{
			++PinsConnnectedCounter;
			return;
		}
	}

	// 金属节点是否已连接
	if (!CreateMaterial->HasMetallicConnected())
	{
		if (TryConnectedMetalic(TextureSampleNode, SelectedTexture, CreateMaterial))
		{
			++PinsConnnectedCounter;
			return;
		}
	}

	// 更多材质节点
	if (!CreateMaterial->HasRoughnessConnected())
	{
		if (TryConnectedRoughness(TextureSampleNode, SelectedTexture, CreateMaterial))
		{
			++PinsConnnectedCounter;
			return;
		}
	}

	if (!CreateMaterial->HasNormalConnected())
	{
		if (TryConnectedNormal(TextureSampleNode, SelectedTexture, CreateMaterial))
		{
			++PinsConnnectedCounter;
			return;
		}
	}

	if (!CreateMaterial->HasAmbientOcclusionConnected())
	{
		if (TryConnectedAmbientOcclusion(TextureSampleNode, SelectedTexture, CreateMaterial))
		{
			++PinsConnnectedCounter;
			return;
		}
	}

	DebugHeader::PrintLog(TEXT("Fail to connect the texture: ") + SelectedTexture->GetName());
}




#pragma endregion 

#pragma region CreateMaterialNodesConnectPins

bool UQuickMaterialCreationWidget::TryConnectedBaseColor(UMaterialExpressionTextureSample* TextureSampleNode,
	UTexture2D* SelectedTexture, UMaterial* CreateMaterial)
{
	for (const FString& BaseColorName : BaseColorArray)
	{
		if (SelectedTexture->GetName().Contains(BaseColorName))
		{
			// 设置"纹理采样"所采样的目标纹理
			TextureSampleNode->Texture = SelectedTexture;

			// 材质节点连接，材质图表添加材质表达式"纹理采样"节点
			CreateMaterial->GetExpressionCollection().AddExpression(TextureSampleNode);

			// 连接 "BaseColor" 节点
			CreateMaterial->GetExpressionInputForProperty(EMaterialProperty::MP_BaseColor)->Expression = TextureSampleNode;

			// 说明材质修改完成
			
			CreateMaterial->PostEditChange();
			// 材质节点连接结束

			// 调整材质节点的位置
			TextureSampleNode->MaterialExpressionEditorX -= 600;
			
			return true;
		}
	}

	return false;
}

bool UQuickMaterialCreationWidget::TryConnectedMetalic(UMaterialExpressionTextureSample* TextureSampleNode,
	UTexture2D* SelectedTexture, UMaterial* CreateMaterial)
{
	for (const FString& MetalicName : MetallicArray)
	{
		if (SelectedTexture->GetName().Contains(MetalicName))
		{
			// 纹理压缩设置
			SelectedTexture->CompressionSettings = TextureCompressionSettings::TC_Default;
			// 纹理 sRGB 设置
			SelectedTexture->SRGB = false;
			// 如果不调用 PostEditChange 可能会导致崩溃，且找不到原因。
			SelectedTexture->PostEditChange();

			// 设置"纹理采样"所采样的目标纹理
			TextureSampleNode->Texture = SelectedTexture;

			// 设置 TextureSample 节点的采样类型，线性颜色
			TextureSampleNode->SamplerType = EMaterialSamplerType::SAMPLERTYPE_LinearColor;

			// 添加表达式
			CreateMaterial->GetExpressionCollection().Expressions.Add(TextureSampleNode);

			// 连接到金属节点
			CreateMaterial->GetExpressionInputForProperty(EMaterialProperty::MP_Metallic)->Expression = TextureSampleNode;

			// 说明材质修改完成
			CreateMaterial->PostEditChange();

			// 调整材质节点的位置
			TextureSampleNode->MaterialExpressionEditorX -= 600;
			TextureSampleNode->MaterialExpressionEditorY += 240;
			return true;
		}
	}

	return false;
}

bool UQuickMaterialCreationWidget::TryConnectedRoughness(UMaterialExpressionTextureSample* TextureSampleNode,
	UTexture2D* SelectedTexture, UMaterial* CreateMaterial)
{
	for (const FString& RoughnessName : RoughnessArray)
	{
		if (SelectedTexture->GetName().Contains(RoughnessName))
		{
			// 纹理压缩设置
			SelectedTexture->CompressionSettings = TextureCompressionSettings::TC_Default;
			// 纹理 sRGB 设置
			SelectedTexture->SRGB = false;
			// 如果不调用 PostEditChange 可能会导致崩溃，且找不到原因。
			SelectedTexture->PostEditChange();
			// 设置"纹理采样"所采样的目标纹理
			TextureSampleNode->Texture = SelectedTexture;

			// 设置 TextureSample 节点的采样类型
			TextureSampleNode->SamplerType = EMaterialSamplerType::SAMPLERTYPE_LinearColor;

			CreateMaterial->GetExpressionCollection().Expressions.Add(TextureSampleNode);
			CreateMaterial->GetExpressionInputForProperty(EMaterialProperty::MP_Roughness)->Expression = TextureSampleNode;
			CreateMaterial->PostEditChange();

			TextureSampleNode->MaterialExpressionEditorX -= 600;
			TextureSampleNode->MaterialExpressionEditorY += 480;
			return true;
		}
	}

	return false;
}

bool UQuickMaterialCreationWidget::TryConnectedNormal(UMaterialExpressionTextureSample* TextureSampleNode,
	UTexture2D* SelectedTexture, UMaterial* CreateMaterial)
{
	for (const FString& NormalName : NormalArray)
	{
		if (SelectedTexture->GetName().Contains(NormalName))
		{
			// 纹理压缩设置
			SelectedTexture->CompressionSettings = TextureCompressionSettings::TC_Normalmap;
			// 纹理 sRGB 设置
			SelectedTexture->SRGB = false;
			// 如果不调用 PostEditChange 可能会导致崩溃，且找不到原因。
			SelectedTexture->PostEditChange();
			// 设置"纹理采样"所采样的目标纹理
			TextureSampleNode->Texture = SelectedTexture;

			// 设置 TextureSample 节点的采样类型
			TextureSampleNode->SamplerType = EMaterialSamplerType::SAMPLERTYPE_Normal;

			CreateMaterial->GetExpressionCollection().Expressions.Add(TextureSampleNode);
			CreateMaterial->GetExpressionInputForProperty(EMaterialProperty::MP_Normal)->Expression = TextureSampleNode;
			CreateMaterial->PostEditChange();

			TextureSampleNode->MaterialExpressionEditorX -= 600;
			TextureSampleNode->MaterialExpressionEditorY += 720;
			return true;
		}
	}

	return false;
}

bool UQuickMaterialCreationWidget::TryConnectedAmbientOcclusion(UMaterialExpressionTextureSample* TextureSampleNode,
	UTexture2D* SelectedTexture, UMaterial* CreateMaterial)
{
	for (const FString& AmbientOcclusionName : AmbientOcclusionArray)
	{
		if (SelectedTexture->GetName().Contains(AmbientOcclusionName))
		{
			// 纹理压缩设置
			SelectedTexture->CompressionSettings = TextureCompressionSettings::TC_Default;
			// 纹理 sRGB 设置
			SelectedTexture->SRGB = false;
			// 如果不调用 PostEditChange 可能会导致崩溃，且找不到原因。
			SelectedTexture->PostEditChange();
			// 设置"纹理采样"所采样的目标纹理
			TextureSampleNode->Texture = SelectedTexture;

			// 设置 TextureSample 节点的采样类型
			TextureSampleNode->SamplerType = EMaterialSamplerType::SAMPLERTYPE_LinearColor;

			CreateMaterial->GetExpressionCollection().Expressions.Add(TextureSampleNode);
			CreateMaterial->GetExpressionInputForProperty(EMaterialProperty::MP_AmbientOcclusion)->Expression = TextureSampleNode;
			CreateMaterial->PostEditChange();

			TextureSampleNode->MaterialExpressionEditorX -= 600;
			TextureSampleNode->MaterialExpressionEditorY += 960;
			return true;
		}
	}

	return false;
}

bool UQuickMaterialCreationWidget::TryConnectedORM(UMaterialExpressionTextureSample* TextureSampleNode,
	UTexture2D* SelectedTexture, UMaterial* CreateMaterial, int32 PinsConnnectedCounter)
{

	// 纹理压缩设置
	SelectedTexture->CompressionSettings = TextureCompressionSettings::TC_Masks;
	// 纹理 sRGB 设置
	SelectedTexture->SRGB = false;
	// 如果不调用 PostEditChange 可能会导致崩溃，且找不到原因。
	SelectedTexture->PostEditChange();
	// 设置"纹理采样"所采样的目标纹理
	TextureSampleNode->Texture = SelectedTexture;

	// 设置 TextureSample 节点的采样类型
	TextureSampleNode->SamplerType = EMaterialSamplerType::SAMPLERTYPE_Masks;

	CreateMaterial->GetExpressionCollection().Expressions.Add(TextureSampleNode);

	//  指定通道的索引来连接到节点
	FExpressionInput* ExpressionInput;
	if (!CreateMaterial->HasAmbientOcclusionConnected())
	{
		ExpressionInput = CreateMaterial->GetExpressionInputForProperty(EMaterialProperty::MP_AmbientOcclusion);
		if (ExpressionInput)
		{
			ExpressionInput->Expression = TextureSampleNode;
			ExpressionInput->Expression->ConnectExpression(ExpressionInput,
				GetORM_ChannelIndex(E_ChannelPackingType_ORM::ECPT_AO));
			++PinsConnnectedCounter;
		}
	}

	if (!CreateMaterial->HasRoughnessConnected())
	{
		ExpressionInput = CreateMaterial->GetExpressionInputForProperty(EMaterialProperty::MP_Roughness);
		if (ExpressionInput)
		{
			ExpressionInput->Expression = TextureSampleNode;
			ExpressionInput->Expression->ConnectExpression(ExpressionInput,
				GetORM_ChannelIndex(E_ChannelPackingType_ORM::ECPT_Roughness));
			++PinsConnnectedCounter;
		}
	}
	
	if (!CreateMaterial->HasMetallicConnected())
	{
		ExpressionInput = CreateMaterial->GetExpressionInputForProperty(EMaterialProperty::MP_Metallic);
		if (ExpressionInput)
		{
			ExpressionInput->Expression = TextureSampleNode;
			ExpressionInput->Expression->ConnectExpression(ExpressionInput,
				GetORM_ChannelIndex(E_ChannelPackingType_ORM::ECPT_Metalic));
			++PinsConnnectedCounter;
		}
	}
	
	CreateMaterial->PostEditChange();

	TextureSampleNode->MaterialExpressionEditorX -= 600;
	TextureSampleNode->MaterialExpressionEditorY += 960;
	return true;
}

int32 UQuickMaterialCreationWidget::GetORM_ChannelIndex(E_ChannelPackingType_ORM OrmType)
{
	switch (OrmType) {
		case E_ChannelPackingType_ORM::ECPT_AO:
			return 1;
		case E_ChannelPackingType_ORM::ECPT_Roughness:
			return 2;
		case E_ChannelPackingType_ORM::ECPT_Metalic:
			return 3;
	}
	return 1;
}
#pragma endregion


#pragma region CreateMaterialInstance
UMaterialInstanceConstant* UQuickMaterialCreationWidget::CreateMaterialInstanceAsset(UMaterial* CreatedMaterial,
	FString NameOfMaterial, const FString& PathToPutMaterialInstance)
{
	NameOfMaterial.RemoveFromStart(MaterialDefaultPrefixName);
	NameOfMaterial.InsertAt(0, MaterialInstanceDefaultPrefixName);
	// 材质实例名称
	const FString NameOfMaterialInstance = NameOfMaterial;

	const FAssetToolsModule& AssetToolsModule = FModuleManager::GetModuleChecked<FAssetToolsModule>(TEXT("AssetTools"));

	// 使用 "UMaterialInstanceConstantFactoryNew" 工厂类来创建"材质实例"
	UMaterialInstanceConstantFactoryNew* MaterialInstanceConstantFactoryNew = NewObject<UMaterialInstanceConstantFactoryNew>();
	
	UObject* CreatedObject = AssetToolsModule.Get().CreateAsset(NameOfMaterialInstance, // 材质实例名称
		PathToPutMaterialInstance, // 材质实例保存路径
		UMaterialInstanceConstant::StaticClass(), // 材质实例类型
		MaterialInstanceConstantFactoryNew); // 材质实例创建的工厂类

	if (UMaterialInstanceConstant* CreatedMaterialInstance = Cast<UMaterialInstanceConstant>(CreatedObject))
	{
		// 设置材质实例的父材质
		CreatedMaterialInstance->SetParentEditorOnly(CreatedMaterial);

		// 通知材质实例编辑完成
		CreatedMaterialInstance->PostEditChange();

		return CreatedMaterialInstance;
	}
	
	return nullptr;
}

#pragma endregion 
