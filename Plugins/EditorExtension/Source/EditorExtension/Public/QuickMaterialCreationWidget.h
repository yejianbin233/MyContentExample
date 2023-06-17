// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EditorUtilityWidget.h"
#include "QuickMaterialCreationWidget.generated.h"

class UMaterialExpressionTextureSample;


// 材质纹理打包类型
UENUM()
enum class E_ChannelPackingType : uint8
{
	ECPT_NoChannelPacking UMETA(DisplayName = "No Channel Packing"),
	ECPT_ORM UMETA(DisplayName = "OcclusionRoughnessMetalic"),
	ECPT_MAX UMETA(DisplayName = "DefaultMax"),
};

// 材质纹理打包类型，定义 ORM（AO/Roughness/Metal）纹理通道
UENUM()
enum class E_ChannelPackingType_ORM : uint8
{
	ECPT_AO UMETA(DisplayName = "Ambient Occlusion"),
	ECPT_Roughness UMETA(DisplayName = "Roughness"),
	ECPT_Metalic UMETA(DisplayName = "Metalic"),
};

/**
 * 快速创建材质编辑器控件
 */
UCLASS()
class EDITOREXTENSION_API UQuickMaterialCreationWidget : public UEditorUtilityWidget
{
	GENERATED_BODY()


public:

#pragma region QuickMaterialCreationCore

	// 材质默认前缀名
	FString MaterialDefaultPrefixName = TEXT("M_");
	// 材质实例默认前缀名
	FString MaterialInstanceDefaultPrefixName = TEXT("MI_");

	// 使用在"内容浏览器"中选择的纹理来创建"材质"、"材质实例"
	UFUNCTION(BlueprintCallable)
	void CreateMaterialFromSelectedTextures();

	// 材质名称（控件面板可自定义的材质名称）
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="CreateMaterialFromSelectedTextures", meta=(EditCondition = "bCustomMaterialName"))
	FString MaterialName = TEXT("M_");

	// 是否自定义材质名称
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="CreateMaterialFromSelectedTextures")
	bool bCustomMaterialName = true;

	// 使用纹理通道来创建材质
	void ORM_CreateMaterialNodes(UMaterial* CreatedMaterial, UTexture2D* SelectedTexture, uint32& PinsConnnectedCounter);
	
#pragma endregion

private:

	
#pragma region QuickMaterialCreation // 创建材质相关内容

	// 处理选择的资产，筛选纹理资产
	bool ProcessSelectedData(const TArray<FAssetData>& SelectedDataToProcess, TArray<UTexture2D*>& OutSelectedTexturesArray, FString& OutSelectedTexturePackagePath);

	// 检查名称是否已经被使用
	bool CheckIsNameUsed(const FString& FolderPathToCheck, const FString& MaterialNameToCheck);

	// 创建材质资产
	UMaterial* CreateMaterialAsset(const FString& NameOfTheMaterial, const FString& PathToPutMaterial);

	// 创建默认的材质资产
	void DefaultCreateMaterialNodes(UMaterial* CreateMaterial, UTexture2D* SelectedTexture, uint32& PinsConnnectedCounter);

	// 旋转纹理贴图的打包类型
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="CreateMaterialFromSelectedTextures", meta=(AllowPrivateAccess))
	E_ChannelPackingType ChannelPackingType = E_ChannelPackingType::ECPT_NoChannelPacking;

	// 是否创建材质实例
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="CreateMaterialFromSelectedTextures", meta=(AllowPrivateAccess))
	bool bCreateMaterialInstance = false;
	
#pragma endregion

#pragma region SupportedTextureNames // 用于配置纹理的类型，不同类型的纹理应该具有与之匹配的名称前/后缀

	// 基础颜色纹理名称的后缀列表，用于判断是否可将纹理连接到 BaseColor 节点
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Supported Texture Names", meta=(AllowPrivateAccess))
	TArray<FString> BaseColorArray = {
		TEXT("_BaseColor"),
		TEXT("_Albedo"),
		TEXT("_Diffuse"),
		TEXT("_diff"),
	};

	// 金属纹理名称的后缀列表，用于判断是否可将纹理连接到 Metal 节点
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Supported Texture Names", meta=(AllowPrivateAccess))
	TArray<FString> MetallicArray = {
		TEXT("_Metallic"),
		TEXT("_metal"),
	};

	// 粗糙度纹理名称的后缀列表，用于判断是否可将纹理连接到 Roughness 节点
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Supported Texture Names", meta=(AllowPrivateAccess))
	TArray<FString> RoughnessArray = {
		TEXT("_Roughness"),
		TEXT("_RoughnessMap"),
		TEXT("_rough"),
	};

	// 法线纹理名称的后缀列表，用于判断是否可将纹理连接到 Normal 节点
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Supported Texture Names", meta=(AllowPrivateAccess))
	TArray<FString> NormalArray = {
		TEXT("_Normal"),
		TEXT("_NormalMap"),
		TEXT("_nor"),
	};

	// 环境遮蔽纹理名称的后缀列表，用于判断是否可将纹理连接到 AmbientOcclusion 节点
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Supported Texture Names", meta=(AllowPrivateAccess))
	TArray<FString> AmbientOcclusionArray = {
		TEXT("_AmbientOcclusion"),
		TEXT("_AmbientOcclusionMap"),
		TEXT("_AO"),
	};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Supported Texture Names", meta=(AllowPrivateAccess))
	TArray<FString> ORMArray = {
		TEXT("_arm"),
		TEXT("_OcclusionRoughnessMetalic"),
		TEXT("_ORM"),
	};
	
#pragma endregion

#pragma region CreateMaterialNodesConnectPins // 创建材质节点并连接

	// 材质图表中进行节点连接
	// 尝试连接材质的基础颜色节点
	bool TryConnectedBaseColor(UMaterialExpressionTextureSample* TextureSampleNode, UTexture2D* SelectedTexture, UMaterial* CreateMaterial);
	bool TryConnectedMetalic(UMaterialExpressionTextureSample* TextureSampleNode, UTexture2D* SelectedTexture, UMaterial* CreateMaterial);
	bool TryConnectedRoughness(UMaterialExpressionTextureSample* TextureSampleNode, UTexture2D* SelectedTexture, UMaterial* CreateMaterial);
	bool TryConnectedNormal(UMaterialExpressionTextureSample* TextureSampleNode, UTexture2D* SelectedTexture, UMaterial* CreateMaterial);
	bool TryConnectedAmbientOcclusion(UMaterialExpressionTextureSample* TextureSampleNode, UTexture2D* SelectedTexture, UMaterial* CreateMaterial);
	bool TryConnectedORM(UMaterialExpressionTextureSample* TextureSampleNode, UTexture2D* SelectedTexture, UMaterial* CreateMaterial, int32 PinsConnnectedCounter);

	int32 GetORM_ChannelIndex(E_ChannelPackingType_ORM OrmType);

#pragma endregion

#pragma region CreateMaterialInstance // 创建材质实例

	// 创建材质实例
	class UMaterialInstanceConstant* CreateMaterialInstanceAsset(UMaterial* CreatedMaterial, FString NameOfMaterial, const FString& PathToPutMaterialInstance);

#pragma endregion 
};
