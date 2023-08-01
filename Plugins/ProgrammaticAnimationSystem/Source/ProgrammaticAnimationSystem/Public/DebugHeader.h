#pragma once
#include "Framework/Notifications/NotificationManager.h"
#include "Misc/MessageDialog.h" // 信息对话框
#include "Widgets/Notifications/SNotificationList.h"

namespace DebugHeader
{
	
	static void Print(const FString& Message, const FColor& Color)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 8.0f, Color, Message);
		}
	}

	static void PrintLog(const FString& Message)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s"), *Message);
	}

	static EAppReturnType::Type ShowMsgDialog(EAppMsgType::Type MsgType, const FString& Message, bool bShowMsgAsWarning = true)
	{
		// 使用弹窗来显示提示信息。
		if (bShowMsgAsWarning)
		{
			const FText MsgTitle = FText::FromString(TEXT("Warning"));

			return FMessageDialog::Open(MsgType, FText::FromString(Message), &MsgTitle);
		}
		else
		{
			return FMessageDialog::Open(MsgType, FText::FromString(Message));
		}
	}

	static void ShowNotifyInfo(const FString& Message)
	{
		FNotificationInfo NotifyInfo(FText::FromString(Message));

		NotifyInfo.bUseLargeFont = true; // 使用大字体
		NotifyInfo.FadeOutDuration = 7.0f; // 淡出时间

		// 使用 "Slate 通知管理器"添加信息通知。
		FSlateNotificationManager::Get().AddNotification(NotifyInfo);
	}

}
