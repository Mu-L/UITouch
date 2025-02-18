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


#include "Widgets/TouchControlWidget.h"
#include "Components/CanvasPanelSlot.h"


void UTouchControlWidget::NativePreConstruct()
{
	Super::NativePreConstruct();
}


void UTouchControlWidget::RemoveTouchDelegate(UTouchComponent* TouchComponent)
{
	Super::RemoveTouchDelegate(TouchComponent);
	for (size_t i = 0; i < TouchLocations.Num(); i++)
	{
		if (TouchLocations[i] != FVector())
		{
			if (TouchComponent)
			{
				TouchComponent->DelegateBind(uint8(TouchLocations[i].Z), false, this, TEXT("TouchMovedLocation"));
			}
		}
	}
}

bool UTouchControlWidget::TouchIndexLocation(const FVector& Location, uint8 FingerIndex)
{
	if (!GetIsEnabled() || GetVisibility() != ESlateVisibility::Visible)  /** * 是否启用,只有可视才能互交 */
	{
		return false;
	}
	int32 Index = GetTouchLocationsIndex(FingerIndex);
	if (Location.Z)
	{
		if (IsTouchLocation(Location) && Index == -1)
		{
			LastTriggerLocation = Location;
			TouchLocations.Add({ Location.X, Location.Y , float(FingerIndex) }); /** * 设置触控位置组的位置 */
			SetIndexTouchDelegate(true, FingerIndex); /** * 绑定移动位置调度器 */
			return true;
		}
	}
	else
	{
		if (Index != -1)
		{
			TouchLocations.RemoveAt(Index); /** * 清除触控位置组的位置 */
			SetIndexTouchDelegate(false, FingerIndex); /** * 解除绑定移动位置调度器 */
		}
	}
	return false;
}

void UTouchControlWidget::TouchMovedLocation(const FVector& Location)
{
	if (!GetIsEnabled())  /** * 是否启用,只有可视才能互交 */
	{
		return;
	}
	if (TouchLocations.Find(Location) != -1)
	{
		return;
	}
	LastTriggerLocation = Location;
	int32 Index = GetTouchLocationsIndex(Location.Z);
	if (Index != -1 && TouchLocations.Num() > Index) /** * 判断是否寻找成功 */
	{
		FVector TouchMovedLocation = Location - TouchLocations[Index];/** * 计算移动位置 */
		TouchLocations[Index] = Location; /** * 覆盖旧位置 */
		if (ClampDifferenceDistance != 0 && FVector2D(TouchMovedLocation).Size() > ClampDifferenceDistance)
		{
			return;
		}
		TouchMovedLocation.Z = Location.Z + 1;
		OnTouchLocation.Broadcast(TouchMovedLocation); /** * 分发移动位置 */
	}
}

void UTouchControlWidget::SetVisibleDisabled(bool bVisible, bool bFlushInput)
{
	Super::SetVisibleDisabled(bVisible, bFlushInput);
	if (bVisible)
	{
		if (bFlushInput && IsDesignTime() == false)
		{
			for (size_t i = 0; i < TouchLocations.Num(); i++)
			{
				SetIndexTouchDelegate(false, uint8(TouchLocations[i].Z)); /** * 解除绑定移动位置调度器 */
			}
			TouchLocations.Empty(); /** * 并清除标记 */
		}
		if (ControlImageWidget)
		{
			ControlImageWidget->SetBrush(DisabledSlateBrush);  /** * 设置背景的图片 */
		}
		TriggerInedxAnimation(-1);
	}
	else
	{
		if (ControlImageWidget)
		{
			ControlImageWidget->SetBrush(ControlSlateBrush);  /** * 设置背景的图片 */
		}
		TriggerInedxAnimation(0);
	}
}

int UTouchControlWidget::GetTouchLocationsIndex(int32 Index)
{
	for (size_t i = 0; i < TouchLocations.Num(); i++)
	{
		if (TouchLocations[i].Z == Index)
		{
			return i;
		}
	}
	return -1;
}
