// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
/**
 * 创建 Slate 控件，根据面板内容，使用 EditorExtension 暴露的接口
 */
class EDITOREXTENSION_API AdvanceDeletionWidget
{
public:
	
};

// 自定义 Slate 用户控件 - SAdvanceDeletionTag
class SAdvanceDeletionTag : public SCompoundWidget
{
	SLATE_BEGIN_ARGS(SAdvanceDeletionTag){}

	// 定义 Slate 控件的参数
	SLATE_ARGUMENT(TArray<TSharedPtr<FAssetData>>, AssetsDataToStore)
	
	SLATE_ARGUMENT(FString, CurrentSelectedFolder)
	
	SLATE_END_ARGS()

protected:

	// 构造面板的表格
	TSharedPtr<SListView<TSharedPtr<FAssetData>>> ConstructedAssetListView;
	
public:
	// Slate 容器的构造函数
	void Construct(const FArguments& InArgs);

protected:

// 表格数据构建
#pragma region RowWidgetForAssetList

	// 为 SListView 创建列表项 菜单控件
	TSharedRef<ITableRow> OnGenerateRowForList(TSharedPtr<FAssetData> AssetDataToDisplay, const TSharedRef<STableViewBase>& OwnerTable);
	
	// 创建菜单列表
	TSharedRef<SListView<TSharedPtr<FAssetData>>> ConstructAssetListView();
	
	// 创建多选框
	TSharedRef<SCheckBox> ConstructCheckBox(const TSharedPtr<FAssetData> AssetDataToDisplay);

	// 创建文本块
	TSharedRef<STextBlock> ConstructTextForRowWidget(const FString& TextContent, const FSlateFontInfo& FontToUse);

	// 创建按钮
	TSharedRef<SButton> ConstructButtonForRowWidget(const TSharedPtr<FAssetData>& AssetData);
	
	// 获取字体
	FSlateFontInfo GetEmboseedTextFont() const { return FCoreStyle::Get().GetFontStyle(FName("EmbossedText"));};

	// 刷新菜单列表
	void RefreshAssetListView();

	// 多选框状态改变时触发
	void OnCheckBoxStateChanged(ECheckBoxState NewState, TSharedPtr<FAssetData> AssetData);

	// 定义按钮点击触发处理函数
	FReply OnDeleteButtonClicked(TSharedPtr<FAssetData> ClickedAssetData);

	// 列表单元行点击事件
	void OnRowWidgetMouseButtonClicked(TSharedPtr<FAssetData> ClickedData);
	
#pragma endregion


// 按钮 
#pragma region TabButtons
	// 一键删除所有选中的资产
	TSharedRef<SButton> ConstructDeleteAllSelectButton();

	// 一键选择所有资产
	TSharedRef<SButton> ConstructSelectAllButton();

	// 一键取消选择所有资产
	TSharedRef<SButton> ConstructDeselectAllButton();

	// 按钮处理逻辑
	// 删除全选
	FReply OnDeleteAllSelectButtonClicked();
	// 选择所有
	FReply OnSelectAllButtonClicked();
	// 反选
	FReply OnDeselectAllButtonClicked();

	// 构造按钮文本块
	TSharedRef<STextBlock> ConstructTextForTabButtons(const FString& TextContent);

#pragma endregion

// 组合框	
#pragma region ComboBoxForListingCondition

	// 构造组合框(下拉框)
	TSharedRef<SComboBox<TSharedPtr<FString>>> ConstructComboBox();

	// 生成组合框内容
	TSharedRef<SWidget> OnGenerateComboContent(TSharedPtr<FString> SourceItem);

	// 当组合框菜单项被选择时调用
	void OnComboSelectionChanged(TSharedPtr<FString> SelectedOption, ESelectInfo::Type InSelectInfo);

	// 文本块
	TSharedPtr<STextBlock> ComboDisplayTextBlock;
	
	// 组合框内容菜单项
	TArray<TSharedPtr<FString>> ComboSourceItems;

	// 构造组合框提示文本
	TSharedRef<STextBlock> ConstructCombeHelpTexts(const FString& TextContent, ETextJustify::Type TextJustify);
	
#pragma endregion 
	
private:

	// 缓存的资产数据
	TArray<TSharedPtr<FAssetData>> StoredAssetsData;

	// 每当按钮被选中时，将其对应的资产添加到该数组中，等待删除
	TArray<TSharedPtr<FAssetData>> AssetsDataToDeleteArray;

	// 复选框按钮数组
	TArray<TSharedRef<SCheckBox>> CheckBoxesArray;

	// 下拉框显示的资产数据数组，用于显示在控件上的资产数据
	TArray<TSharedPtr<FAssetData>> DisplayedAssetsData;
};
