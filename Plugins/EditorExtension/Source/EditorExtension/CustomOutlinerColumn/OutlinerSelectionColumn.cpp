 // Fill out your copyright notice in the Description page of Project Settings.
#include "OutlinerSelectionColumn.h"
#include "EditorExtension/CustomStyle/EditorExtensionStyle.h"
#include "ActorTreeItem.h"
#include "EditorExtension/Public/EditorExtension.h"

 FOutlinerSelectionLockColumn::FOutlinerSelectionLockColumn(ISceneOutliner& SceneOutlinerColumn)
 {
 	
 }

 SHeaderRow::FColumn::FArguments FOutlinerSelectionLockColumn::ConstructHeaderRowColumn()
 {
 	
	SHeaderRow::FColumn::FArguments ConstructedHeaderRow = SHeaderRow::Column(GetColumnID())
		.FixedWidth(24.0f) // 尺寸
 		.HAlignHeader(HAlign_Center) // 水平布局
 		.VAlignHeader(VAlign_Center) // 垂直布局
 		.HAlignCell(HAlign_Center)
 		.VAlignCell(VAlign_Center)
 		.DefaultTooltip(FText::FromString(TEXT("Actor Selection Lock - Press icon to lock actor selection"))) // 提示内容
		[
			SNew(SImage)
				.ColorAndOpacity(FSlateColor::UseForeground())
				.Image(FEditorExtensionStyle::GetCreatedSlateStyleSet()->GetBrush(FEditorExtensionStyle::GetStyleSetLevelEditor_LockSelectionIconName()))
		]
 	;
	
 	return ConstructedHeaderRow;
 }

 const TSharedRef<SWidget> FOutlinerSelectionLockColumn::ConstructRowWidget(FSceneOutlinerTreeItemRef TreeItem,
	 const STableRow<FSceneOutlinerTreeItemPtr>& Row)
 {
 	// 大概菜单行所代表的场景 Actor - FActorTreeItem
 	FActorTreeItem* ActorTreeItem = TreeItem->CastTo<FActorTreeItem>();

    if (!ActorTreeItem || !ActorTreeItem->IsValid())
    {
	    return SNullWidget::NullWidget;
    }

 	FEditorExtensionModule& ExtensionModule = FModuleManager::Get().LoadModuleChecked<FEditorExtensionModule>(TEXT("EditorExtension"));

 	// 检查 Actor 是否已经被锁定，被锁定了就更新复选框状态
 	const bool bIsActorSelectionLocked = ExtensionModule.CheckIsActorSelectionLocked(ActorTreeItem->Actor.Get());

 	const TAttribute<ECheckBoxState> CheckedState = bIsActorSelectionLocked ? ECheckBoxState::Checked : ECheckBoxState::Unchecked;

 	const FCheckBoxStyle& ToggleButtonStyle = FEditorExtensionStyle::GetCreatedSlateStyleSet()->GetWidgetStyle<FCheckBoxStyle>(FEditorExtensionStyle::GetStyleSetLevelEditor_LockSelectionIconName());
 	
 	TSharedRef<SCheckBox> ConstructedRowWidgetCheckBox =
 		SNew(SCheckBox)
 			.Visibility(EVisibility::Visible)
 			//.Type(ESlateCheckBoxType::ToggleButton) // 切换按钮 - 有问题，无法显示按钮
 			//.Style(&ToggleButtonStyle)
 			.HAlign(HAlign_Center)
 			.IsChecked(CheckedState) // 更新状态
 			.OnCheckStateChanged(this, &FOutlinerSelectionLockColumn::OnRowWidgetCheckStateChanged, ActorTreeItem->Actor) //  状态改变时触发绑定事件
	 		;
 	
 	return ConstructedRowWidgetCheckBox;
 }

 void FOutlinerSelectionLockColumn::OnRowWidgetCheckStateChanged(ECheckBoxState NewState,
	 TWeakObjectPtr<AActor> CorrespondingActor)
 {
 	FEditorExtensionModule& ExtensionModule = FModuleManager::Get().LoadModuleChecked<FEditorExtensionModule>(TEXT("EditorExtension"));
 	
	 switch (NewState) {
	 	case ECheckBoxState::Unchecked:
	 		ExtensionModule.ProcessLockingForOutliner(CorrespondingActor.Get(), false);
	 		break;
	 	case ECheckBoxState::Checked:
	 		ExtensionModule.ProcessLockingForOutliner(CorrespondingActor.Get(), true);
	 		break;
	 	case ECheckBoxState::Undetermined:
	 		
	 		break;
	 	default: ;
	 }
 }
