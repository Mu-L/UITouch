/**
 * Copyright: Aimo_皑墨
 * Open source protocol: MIT License
 * Open Source Date: October 2, 2022
 * BiLiBiLi (哔哩哔哩) address: https://space.bilibili.com/146962867
 * making address: https://github.com/AimoTvT/UITouch
 * We welcome the contributions of powerful movers and movers to join this plugin
 * Build powerful plugins together!!
 *
 * 版权所有权: Aimo_皑墨
 * 开源协议: MIT License
 * 开源时间: 2022年10月2号
 * BiLiBiLi(哔哩哔哩)地址: https://space.bilibili.com/146962867
 * GitHub地址: https://github.com/AimoTvT/UITouch
 * 欢迎有实力的大佬/萌新加入本插件的贡献
 * 一起打造强大的插件!!!
 */



#include "Widgets/TouchDetectionWidget.h"
#include "Components/CanvasPanelSlot.h"



void UTouchDetectionWidget::RemoveTouchDelegate(UTouchComponent* TouchComponent)
{
	Super::RemoveTouchDelegate(TouchComponent);
	if (TouchFingerIndex != 255)
	{
		if (TouchComponent)
		{
			TouchComponent->DelegateBind(TouchFingerIndex, false, this, TEXT("TouchMovedLocation"));
		}
	}
}

void UTouchDetectionWidget::TouchMovedLocation(const FVector& Location)
{
	if (!GetIsEnabled() || GetVisibility() != ESlateVisibility::Visible)  /** * 是否启用,只有可视才能互交 */
	{
		return;
	}
	if (LastTriggerLocation == Location)
	{
		return;
	}
	Super::TouchMovedLocation(Location);
	if (Location.Z >= 0.0 && IsTouchLocation(Location))
	{
		if (bDetectionTouch == false)
		{
			bDetectionTouch = true;
			OnTouchLocation.Broadcast({ LastTriggerLocation.X, LastTriggerLocation.Y, LastTriggerLocation.Z + 1 }); /** * 触发触摸位置 */
			if (DetectionImageWidget)
			{
				DetectionImageWidget->SetBrush(TriggerDetectionSlateBrush);  /** * 设置的图片 */
				UCanvasPanelSlot* UpSpeedCanvasPanelSlot = Cast<UCanvasPanelSlot>(DetectionImageWidget->Slot);
				if (UpSpeedCanvasPanelSlot)
				{
					UpSpeedCanvasPanelSlot->SetSize(TriggerDetectionSlateBrush.GetImageSize()); /** * 设置图片大小 */
				}
			}
			TriggerInedxAnimation(1);
		}
	}
	else
	{
		if (bDetectionTouch)
		{
			bDetectionTouch = false;
			OnTouchLocation.Broadcast({ LastTriggerLocation.X, LastTriggerLocation.Y, 0.0 }); /** * 触发触摸位置 */
			if (DetectionImageWidget)
			{
				DetectionImageWidget->SetBrush(DetectionSlateBrush);  /** * 设置的图片 */
				UCanvasPanelSlot* UpSpeedCanvasPanelSlot = Cast<UCanvasPanelSlot>(DetectionImageWidget->Slot);
				if (UpSpeedCanvasPanelSlot)
				{
					UpSpeedCanvasPanelSlot->SetSize(DetectionSlateBrush.GetImageSize()); /** * 设置图片大小 */
				}
			}
			TriggerInedxAnimation(0);
		}
	}
}

void UTouchDetectionWidget::SetVisibleDisabled(bool bVisible, bool bFlushInput)
{
	Super::SetVisibleDisabled(bVisible, bFlushInput);
	if (bVisible)
	{
		if (bFlushInput && IsDesignTime() == false)
		{
			if (TouchFingerIndex != 255)
			{
				SetIndexTouchDelegate(false, TouchFingerIndex);
				TouchFingerIndex = 255;
			}
		}
		if (DetectionImageWidget)
		{
			DetectionImageWidget->SetBrush(DisabledSlateBrush);  /** * 设置按下的图片 */
			UCanvasPanelSlot* DetectionCanvasPanelSlot = Cast<UCanvasPanelSlot>(DetectionImageWidget->Slot);  /** * 获取画布 */
			if (DetectionCanvasPanelSlot)
			{
				DetectionCanvasPanelSlot->SetSize(DisabledSlateBrush.GetImageSize());  /** * 设置大小 */
			}
		}
		TriggerInedxAnimation(-1);
	}
	else
	{
		if (DetectionImageWidget)
		{
			DetectionImageWidget->SetBrush(DetectionSlateBrush);  /** * 设置按下的图片 */
			UCanvasPanelSlot* DetectionCanvasPanelSlot = Cast<UCanvasPanelSlot>(DetectionImageWidget->Slot);  /** * 获取画布 */
			if (DetectionCanvasPanelSlot)
			{
				DetectionCanvasPanelSlot->SetSize(DetectionSlateBrush.GetImageSize());  /** * 设置大小 */
			}
		}
		TriggerInedxAnimation(0);
	}
}

void UTouchDetectionWidget::SetOnIndexTouchDelegate(uint8 FingerIndex)
{
	if (!GetIsEnabled())
	{
		return;
	}
	if (FingerIndex != 255)
	{
		if (TouchFingerIndex != 255)
		{
			SetIndexTouchDelegate(false, TouchFingerIndex);
		}
		TouchFingerIndex = FingerIndex;
		SetIndexTouchDelegate(true, TouchFingerIndex);
	}
	else
	{
		SetIndexTouchDelegate(false, TouchFingerIndex);
		TouchMovedLocation({ LastTriggerLocation.X, LastTriggerLocation.Y, -1.0 });
		TouchFingerIndex = 255;
	}
}
