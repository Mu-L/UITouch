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


#include "Widgets/TouchButtonWidget.h"
#include "Components/CanvasPanelSlot.h"


void UTouchButtonWidget::RemoveTouchDelegate(UTouchComponent* TouchComponent)
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

bool UTouchButtonWidget::TouchIndexLocation(const FVector& Location, uint8 FingerIndex)
{
	if (!GetIsEnabled() || GetVisibility() != ESlateVisibility::Visible)  /** * 是否启用,只有可视才能互交 */
	{
		if (FingerIndex == 255 || TouchFingerIndex != FingerIndex)
		{
			return false;
		}
	}
	if (TouchFingerIndex == 255 && IsTouchLocation(Location))  /** * 触控索引是否空 && 是否进入触控区域 */
	{
		LastTriggerLocation = Location;
		if (bPressedHandover)
		{
			if (Location.Z)
			{
				bPressed = !bPressed;
				OnTouchLocation.Broadcast({ Location.X, Location.Y, bPressed ? FingerIndex + 1.0 : 0.0 });
				if (ButtonImageWidget)
				{
					ButtonImageWidget->SetBrush(bPressed ? PressedButtonSlateBrush : ButtonSlateBrush);
					UCanvasPanelSlot* UpSpeedCanvasPanelSlot = Cast<UCanvasPanelSlot>(ButtonImageWidget->Slot);
					if (UpSpeedCanvasPanelSlot)
					{
						UpSpeedCanvasPanelSlot->SetSize((bPressed ? PressedButtonSlateBrush : ButtonSlateBrush).GetImageSize()); /** * 设置图片大小 */
					}
				}
				TriggerInedxAnimation(bPressed ? 1 : 0);
			}
			return true;
		}
		else
		{
			bPressed = true;
			TouchFingerIndex = FingerIndex;
			OnTouchLocation.Broadcast({ Location.X, Location.Y, FingerIndex + 1.0 });
			if (ButtonImageWidget)
			{
				ButtonImageWidget->SetBrush(PressedButtonSlateBrush);
				UCanvasPanelSlot* UpSpeedCanvasPanelSlot = Cast<UCanvasPanelSlot>(ButtonImageWidget->Slot);
				if (UpSpeedCanvasPanelSlot)
				{
					UpSpeedCanvasPanelSlot->SetSize(PressedButtonSlateBrush.GetImageSize()); /** * 设置图片大小 */
				}
			}
			TriggerInedxAnimation(1);
			return true;
		}
	}
	else
	{
		if (TouchFingerIndex == FingerIndex)  /** * 判断是否是第二次松下触控 */
		{
			TouchFingerIndex = 255;
			bPressed = false;
			OnTouchLocation.Broadcast({ LastTriggerLocation.X, LastTriggerLocation.Y,  0.0 });
			if (ButtonImageWidget)
			{
				ButtonImageWidget->SetBrush(ButtonSlateBrush);
				UCanvasPanelSlot* UpSpeedCanvasPanelSlot = Cast<UCanvasPanelSlot>(ButtonImageWidget->Slot);
				if (UpSpeedCanvasPanelSlot)
				{
					UpSpeedCanvasPanelSlot->SetSize(ButtonSlateBrush.GetImageSize()); /** * 设置图片大小 */
				}
			}
			TriggerInedxAnimation(0);
		}
	}
	return false;
}

void UTouchButtonWidget::SetVisibleDisabled(bool bVisible, bool bFlushInput)
{
	Super::SetVisibleDisabled(bVisible, bFlushInput);
	if (bVisible)
	{
		if (bFlushInput && IsDesignTime() == false)
		{
			if (bPressed)
			{
				SetIndexTouchDelegate(false, TouchFingerIndex);
				bPressed = false;
			}
		}
		if (ButtonImageWidget)
		{
			ButtonImageWidget->SetBrush(DisabledSlateBrush);  /** * 设置按下的图片 */
			UCanvasPanelSlot* ButtonCanvasPanelSlot = Cast<UCanvasPanelSlot>(ButtonImageWidget->Slot);  /** * 获取画布 */
			if (ButtonCanvasPanelSlot)
			{
				ButtonCanvasPanelSlot->SetSize(DisabledSlateBrush.GetImageSize());  /** * 设置大小 */
			}
		}
		TriggerInedxAnimation(-1);
	}
	else
	{
		if (ButtonImageWidget)
		{
			ButtonImageWidget->SetBrush(bPressed ? PressedButtonSlateBrush : ButtonSlateBrush);  /** * 设置按下的图片 */
			UCanvasPanelSlot* ButtonCanvasPanelSlot = Cast<UCanvasPanelSlot>(ButtonImageWidget->Slot);  /** * 获取画布 */
			if (ButtonCanvasPanelSlot)
			{
				ButtonCanvasPanelSlot->SetSize(bPressed ? PressedButtonSlateBrush.GetImageSize() : ButtonSlateBrush.GetImageSize());  /** * 设置大小 */
			}
		}
		TriggerInedxAnimation(0);
	}
}
