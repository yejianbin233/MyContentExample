// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ISceneOutlinerColumn.h"

/**
 * 
 */
class EDITOREXTENSION_API FOutlinerSelectionLockColumn : public ISceneOutlinerColumn // 实现接口 ISceneOutlinerColumn
{
public:
	FOutlinerSelectionLockColumn(ISceneOutliner& SceneOutlinerColumn); // 定义构造函数

	// 必须重载的方法 1 - 定义 列的 ID（唯一）
	virtual FName GetColumnID() override { return FName("SelectionLock"); };

	// 必须创建该函数，否则编译错误。
	static FName GetID() { return FName("SelectionLock");};

	// 必须重载的方法 2 - 定义表头行的列
	virtual SHeaderRow::FColumn::FArguments ConstructHeaderRowColumn() override;

	// 必须重载的方法 3 - 定义表格单元行的列
	virtual const TSharedRef<SWidget> ConstructRowWidget(FSceneOutlinerTreeItemRef TreeItem, const STableRow<FSceneOutlinerTreeItemPtr>& Row) override;
	
	void OnRowWidgetCheckStateChanged(ECheckBoxState NewState, TWeakObjectPtr<AActor> CorrespondingActor);
};
