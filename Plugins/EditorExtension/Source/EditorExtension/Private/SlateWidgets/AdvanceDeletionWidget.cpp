// Fill out your copyright notice in the Description page of Project Settings.


#include "SlateWidgets/AdvanceDeletionWidget.h"

#include "DebugHeader.h"
#include "SlateBasics.h"
#include "EditorExtension.h"

#define ListAll TEXT("List All Available Assets")
#define ListUnused TEXT("List Unused Assets")
#define ListSameName TEXT("List Assets With Same Name")

void SAdvanceDeletionTag::Construct(const FArguments& InArgs)
{
	bCanSupportFocus = true; // 支持聚焦

	// 字体，EmbossedText 应该在字体库有对应的命名，需要匹配。
	FSlateFontInfo TitleTextFont = GetEmboseedTextFont();
	TitleTextFont.Size = 30;

	// 获取传递的参数设置变量
	StoredAssetsData = InArgs._AssetsDataToStore;

	DisplayedAssetsData = StoredAssetsData;
	
	// 每次构造时重置 CheckBoxesArray 的数据
	CheckBoxesArray.Empty();
	// 每次构造时重置 AssetsDataToDeleteArray 的数据
	AssetsDataToDeleteArray.Empty();

	// 每次构造时重置下拉框菜单项的 ComboSourceItems 数据
	ComboSourceItems.Empty();

	// 初始化下拉框的菜单项
	ComboSourceItems.Add(MakeShared<FString>(ListAll));
	ComboSourceItems.Add(MakeShared<FString>(ListUnused));
	ComboSourceItems.Add(MakeShared<FString>(ListSameName));
	
	ChildSlot[
		SNew(SVerticalBox) // 水平框控件
			+SVerticalBox::Slot()
				.AutoHeight() // 自动高度
				[
					SNew(STextBlock) // 字体控件
						.Text(FText::FromString(TEXT("Advance Deletion")))
						.Font(TitleTextFont) // 设置字体
						.Justification(ETextJustify::Center) // 文本对齐
						.ColorAndOpacity(FColor::White) // 字体颜色和不透明度
				]

			+SVerticalBox::Slot()
				.AutoHeight()
				[
					SNew(SHorizontalBox)
						+SHorizontalBox::Slot()
							.AutoWidth()
							[
								ConstructComboBox()
							]

						+SHorizontalBox::Slot()
							.FillWidth(0.6f)
							[
								ConstructCombeHelpTexts(TEXT("Specify the listing condition in the drop down. Left mouse click to go to where asset is located"), ETextJustify::Center)
							]

						+SHorizontalBox::Slot()
							.FillWidth(0.1f)
							[
								ConstructCombeHelpTexts(TEXT("Current Folder: \n") + InArgs._CurrentSelectedFolder, ETextJustify::Right)
							]	
				]

			+SVerticalBox::Slot()
				.VAlign(VAlign_Fill)
				[
					SNew(SScrollBox)
						+SScrollBox::Slot()
						[
							ConstructAssetListView()
						]
				]

			+SVerticalBox::Slot()
				.AutoHeight()
				[
					SNew(SHorizontalBox)
						+SHorizontalBox::Slot()
						.FillWidth(10.0f)
						.Padding(5.0f)
						[
							ConstructDeleteAllSelectButton()
						]

						+SHorizontalBox::Slot()
							.FillWidth(10.0f)
							.Padding(5.0f)
							[
								ConstructSelectAllButton()
							]

						+SHorizontalBox::Slot()
							.FillWidth(10.0f)
							.Padding(5.0f)
							[
								ConstructDeselectAllButton()
							]
				]
	];
}

#pragma region TabButtons

TSharedRef<SButton> SAdvanceDeletionTag::ConstructDeleteAllSelectButton()
{
	TSharedRef<SButton> ConstructDeleteAllButton =
		SNew(SButton)
			.ContentPadding(FMargin(5.0f))
			.OnClicked(this, &SAdvanceDeletionTag::OnDeleteAllSelectButtonClicked);
	
	ConstructDeleteAllButton->SetContent(ConstructTextForTabButtons(TEXT("Delete All")));
	
	return ConstructDeleteAllButton;
}

TSharedRef<SButton> SAdvanceDeletionTag::ConstructSelectAllButton()
{
	TSharedRef<SButton> ConstructSelectAllButton =
		SNew(SButton)
			.ContentPadding(FMargin(5.0f))
			.OnClicked(this, &SAdvanceDeletionTag::OnSelectAllButtonClicked);
	
	ConstructSelectAllButton->SetContent(ConstructTextForTabButtons(TEXT("Select All")));

	return ConstructSelectAllButton;
}

TSharedRef<SButton> SAdvanceDeletionTag::ConstructDeselectAllButton()
{
	TSharedRef<SButton> ConstructDeselectAllButton = 
		SNew(SButton)
			.ContentPadding(FMargin(5.0f))
			.OnClicked(this, &SAdvanceDeletionTag::OnDeselectAllButtonClicked);
			
	ConstructDeselectAllButton->SetContent(ConstructTextForTabButtons(TEXT("Deselect All")));
	
	return ConstructDeselectAllButton;
}

FReply SAdvanceDeletionTag::OnDeleteAllSelectButtonClicked()
{
	if (AssetsDataToDeleteArray.Num() == 0)
	{
		DebugHeader::ShowMsgDialog(EAppMsgType::Ok, TEXT("No Asset currently selected"), false);
		return FReply::Handled();
	}

	TArray<FAssetData> AssetsDataToDelete;

	for (TSharedPtr<FAssetData> AssetDataToDelete : AssetsDataToDeleteArray)
	{
		AssetsDataToDelete.AddUnique(*AssetDataToDelete.Get());
	}

	FEditorExtensionModule& EditorExtensionModule = FModuleManager::LoadModuleChecked<FEditorExtensionModule>(TEXT("EditorExtension"));

	const bool bAssetsDeleted = EditorExtensionModule.DeleteMultipleAssetsForAssetList(AssetsDataToDelete);

	if (bAssetsDeleted)
	{
		for (TSharedPtr<FAssetData> DeleteData : AssetsDataToDeleteArray)
		{
			if (StoredAssetsData.Contains(DeleteData))
			{
				StoredAssetsData.Remove(DeleteData);
			}
			
			if (DisplayedAssetsData.Contains(DeleteData))
			{
				DisplayedAssetsData.Remove(DeleteData);
			}
		}

		
		RefreshAssetListView();
	}
	
	return FReply::Handled();
}


FReply SAdvanceDeletionTag::OnSelectAllButtonClicked()
{
	if (CheckBoxesArray.Num() == 0)
	{
		DebugHeader::ShowMsgDialog(EAppMsgType::Ok, TEXT("No Asset to selected"), false);
		FReply::Handled();
	}

	for (TSharedRef<SCheckBox>& CheckBox : CheckBoxesArray)
	{
		if (CheckBox->IsChecked())
		{
			// 如果已选择，则跳过
			continue;
		}

		// 切换选中状态
		CheckBox->ToggleCheckedState();
	}
	
	return FReply::Handled();
}

FReply SAdvanceDeletionTag::OnDeselectAllButtonClicked()
{
	if (CheckBoxesArray.Num() == 0)
	{
		DebugHeader::ShowMsgDialog(EAppMsgType::Ok, TEXT("No Asset to deselected"), false);
		FReply::Handled();
	}

	for (TSharedRef<SCheckBox>& CheckBox : CheckBoxesArray)
	{
		if (CheckBox->IsChecked())
		{
			// 切换选中状态
			CheckBox->ToggleCheckedState();
		}
	}
	
	return FReply::Handled();
}

TSharedRef<STextBlock> SAdvanceDeletionTag::ConstructTextForTabButtons(const FString& TextContent)
{
	FSlateFontInfo ButtonTextFont = GetEmboseedTextFont();
	ButtonTextFont.Size = 15.0f;
	
	TSharedRef<STextBlock> ConstructedTextBlock =
		SNew(STextBlock)
		.Text(FText::FromString(TextContent))
		.Font(ButtonTextFont)
		.Justification(ETextJustify::Center);

	return ConstructedTextBlock;
}

#pragma endregion 

#pragma region RowWidgetForAssetList

TSharedRef<ITableRow> SAdvanceDeletionTag::OnGenerateRowForList(TSharedPtr<FAssetData> AssetDataToDisplay
															, const TSharedRef<STableViewBase>& OwnerTable)
{
	// 如果资产无效
	if (!AssetDataToDisplay.IsValid())
	{
		return SNew(STableRow<TSharedPtr<FAssetData>>, OwnerTable);
	}

	// 资产类型
	const FString DisplayAssetClassName = AssetDataToDisplay->AssetClassPath.GetAssetName().ToString();
	// 资产名称
	const FString DisplayAssetName = AssetDataToDisplay->AssetName.ToString();
	FSlateFontInfo AssetClassNameFont = GetEmboseedTextFont();
	AssetClassNameFont.Size = 10;
	FSlateFontInfo AssetNameFont = GetEmboseedTextFont();
	AssetNameFont.Size = 15;
	
	TSharedRef<STableRow<TSharedPtr<FAssetData>>> ListViewRowWidget = SNew(STableRow<TSharedPtr<FAssetData>>, OwnerTable)
		.Padding(FMargin(5.0f)) // 间隔
		[
			SNew(SHorizontalBox)
				+SHorizontalBox::Slot()
					.HAlign(HAlign_Left) // 水平左对齐
					.VAlign(VAlign_Center) // 垂直中心对齐
					.FillWidth(0.05f)
					[
						ConstructCheckBox(AssetDataToDisplay)
					]
				+SHorizontalBox::Slot()
					.HAlign(HAlign_Center)
					.VAlign(VAlign_Fill)
					.FillWidth(0.5f)
					[
						ConstructTextForRowWidget(DisplayAssetClassName, AssetClassNameFont)
					]
				+SHorizontalBox::Slot()
					.HAlign(HAlign_Left)
					.VAlign(VAlign_Fill)
					[
						ConstructTextForRowWidget(DisplayAssetName, AssetNameFont)
					]
				+SHorizontalBox::Slot()
					.HAlign(HAlign_Right)
					.VAlign(VAlign_Fill)
					[
						ConstructButtonForRowWidget(AssetDataToDisplay)
					]
		];

	return ListViewRowWidget;
}

TSharedRef<SListView<TSharedPtr<FAssetData>>> SAdvanceDeletionTag::ConstructAssetListView()
{
	
	this->ConstructedAssetListView = SNew(SListView<TSharedPtr<FAssetData>>) // 列表控件
							.ItemHeight(24) // 高度设置
							.ListItemsSource(&DisplayedAssetsData) // 设置数据源
							.OnGenerateRow(this, &SAdvanceDeletionTag::OnGenerateRowForList) // 根据数据源生成列表项
							.OnMouseButtonClick(this, &SAdvanceDeletionTag::OnRowWidgetMouseButtonClicked); // 绑定列表单元行点击事件
	return this->ConstructedAssetListView.ToSharedRef();
}

TSharedRef<SCheckBox> SAdvanceDeletionTag::ConstructCheckBox(const TSharedPtr<FAssetData> AssetDataToDisplay)
{
	TSharedRef<SCheckBox> ConstructedCheckBox = SNew(SCheckBox)
		.Type(ESlateCheckBoxType::CheckBox) // 复选框类型
		.OnCheckStateChanged(this, &SAdvanceDeletionTag::OnCheckBoxStateChanged, AssetDataToDisplay)// 复选框状态改变委托
		.Visibility(EVisibility::Visible); // 可见性

	// 在构造复选框时，将其添加到复选框数组中。
	CheckBoxesArray.Add(ConstructedCheckBox);
	
	return ConstructedCheckBox;
}

TSharedRef<STextBlock> SAdvanceDeletionTag::ConstructTextForRowWidget(const FString& TextContent,
	const FSlateFontInfo& FontToUse)
{
	TSharedRef<STextBlock> ConstructedTextBlock = SNew(STextBlock)
		.Text(FText::FromString(TextContent))
		.Font(FontToUse)
		.ColorAndOpacity(FColor::White);

	return ConstructedTextBlock;
}

TSharedRef<SButton> SAdvanceDeletionTag::ConstructButtonForRowWidget(const TSharedPtr<FAssetData>& AssetData)
{
	TSharedRef<SButton> ConstructedButton =
		SNew(SButton)
			.Text(FText::FromString(TEXT("Delete")))
			.OnClicked(this, &SAdvanceDeletionTag::OnDeleteButtonClicked, AssetData);
				
			

	return ConstructedButton;
}

void SAdvanceDeletionTag::RefreshAssetListView()
{
	// 每次需要刷新资产列表时，都先将待删除数组清空。
	AssetsDataToDeleteArray.Empty();
	// 每次需要刷新资产列表时，都需要先将复选框按钮数组清空，在 RebuildList 时会自动添加。
	CheckBoxesArray.Empty();

	if (ConstructedAssetListView.IsValid())
	{
		// 重新构建列表
		ConstructedAssetListView->RebuildList();
	}
}

void SAdvanceDeletionTag::OnCheckBoxStateChanged(ECheckBoxState NewState, TSharedPtr<FAssetData> AssetData)
{
	switch (NewState) {
	case ECheckBoxState::Unchecked:

		// 当按钮被取消选中时，判断其资产是否在待删除数组中，如果在则将其移出。
		if (AssetsDataToDeleteArray.Contains(AssetData))
		{
			AssetsDataToDeleteArray.Remove(AssetData);
		}
		
		DebugHeader::PrintLog(AssetData->AssetName.ToString() + TEXT(" is unchecked"));
		break;

	case ECheckBoxState::Checked:
		// 当按钮被选中时，将其资产添加到待删除数组中
		AssetsDataToDeleteArray.AddUnique(AssetData);
		DebugHeader::PrintLog(AssetData->AssetName.ToString() + TEXT(" is checked"));
		break;

	case ECheckBoxState::Undetermined:
		break;

	default: ;
	}
}

FReply SAdvanceDeletionTag::OnDeleteButtonClicked(TSharedPtr<FAssetData> ClickedAssetData)
{
	FEditorExtensionModule& EditorExtensionModule = FModuleManager::LoadModuleChecked<FEditorExtensionModule>(TEXT("EditorExtension"));

	// 删除资产
	const bool bAssetDeleted = EditorExtensionModule.DeleteSingleAssetForAssetList(*ClickedAssetData.Get());

	if (bAssetDeleted)
	{
		// 成功删除后需要: 1. 更新缓存数据 2. 刷新界面
		if (this->StoredAssetsData.Contains(ClickedAssetData))
		{
			StoredAssetsData.Remove(ClickedAssetData);
		}

		if (this->DisplayedAssetsData.Contains(ClickedAssetData))
		{
			DisplayedAssetsData.Remove(ClickedAssetData);
		}
		
		RefreshAssetListView();
	}
	
	return FReply::Handled();
}

void SAdvanceDeletionTag::OnRowWidgetMouseButtonClicked(TSharedPtr<FAssetData> ClickedData)
{
	FEditorExtensionModule& EditorExtensionModule = FModuleManager::LoadModuleChecked<FEditorExtensionModule>(TEXT("EditorExtension"));

	// 当点击列表菜单行时，会在内容浏览器同步选中该资产。
	EditorExtensionModule.SyncCBToClickedAssetForAssetList(ClickedData.Get()->GetObjectPathString());
}

#pragma endregion

#pragma region ComboBoxForListingCondition
TSharedRef<SComboBox<TSharedPtr<FString>>> SAdvanceDeletionTag::ConstructComboBox()
{
	TSharedRef<SComboBox<TSharedPtr<FString>>> ConstructedComboBox =
		SNew(SComboBox<TSharedPtr<FString>>)
		.OptionsSource(&ComboSourceItems)
		.OnGenerateWidget(this, &SAdvanceDeletionTag::OnGenerateComboContent)
		.OnSelectionChanged(this, &SAdvanceDeletionTag::OnComboSelectionChanged)
		[
			// 组合框的默认插槽
			SAssignNew(ComboDisplayTextBlock, STextBlock)
				.Text(FText::FromString(TEXT("List Assets Option")))
		];

	return ConstructedComboBox;
}

TSharedRef<SWidget> SAdvanceDeletionTag::OnGenerateComboContent(TSharedPtr<FString> SourceItem)
{
	TSharedRef<STextBlock> ConstructedComboText =
		SNew(STextBlock)
			.Text(FText::FromString(*SourceItem.Get()));

	return ConstructedComboText;
}

void SAdvanceDeletionTag::OnComboSelectionChanged(TSharedPtr<FString> SelectedOption, ESelectInfo::Type InSelectInfo)
{
	FEditorExtensionModule& EditorExtensionModule = FModuleManager::LoadModuleChecked<FEditorExtensionModule>(TEXT("EditorExtension"));
	
	ComboDisplayTextBlock->SetText(FText::FromString(*SelectedOption.Get()));

	// 根据下拉框选项筛选不同条件下可显示的资产
	if (*SelectedOption.Get() == ListAll)
	{
		// 所有存储的资产数据（Stored Asset Data）
		DisplayedAssetsData = StoredAssetsData;
		RefreshAssetListView();
	}
	else if(*SelectedOption.Get() == ListUnused)
	{
		// 所有未被使用的资产数据
		
		EditorExtensionModule.ListUnusedAssetsForAssetList(StoredAssetsData, DisplayedAssetsData);

		RefreshAssetListView();
	}
	else if(*SelectedOption.Get() == ListSameName)
	{
		EditorExtensionModule.ListSameNameAssetsForAssetList(StoredAssetsData, DisplayedAssetsData);
		RefreshAssetListView();
	}
}

TSharedRef<STextBlock> SAdvanceDeletionTag::ConstructCombeHelpTexts(const FString& TextContent,
	ETextJustify::Type TextJustify)
{
	TSharedRef<STextBlock> ConstructedHelpText =
		SNew(STextBlock)
			.Text(FText::FromString(TextContent))
			.Justification(TextJustify)
			.AutoWrapText(true) // 自动换行
		;

	return ConstructedHelpText;
}

#pragma endregion 
