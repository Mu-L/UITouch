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


#include "Widgets/TouchWidget.h"
#include "Components/PanelWidget.h"
#include "Kismet/KismetMathLibrary.h" //官方函数库
#include "Runtime/Engine/Public/DelayAction.h" //延迟的函数库
#include "Runtime/UMG/Public/Blueprint/WidgetLayoutLibrary.h"


UTouchWidget::UTouchWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	SetVisibilityInternal(ESlateVisibility::Visible);
}


void UTouchWidget::NativePreConstruct()
{
	Super::NativePreConstruct();
	SetVisibleDisabled(bIsEnabled == 0);
}

void UTouchWidget::NativeConstruct()
{
	Super::NativeConstruct();

}

void UTouchWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	if (bCustomTrigger == false)
	{
		BindTouchDelegate();
	}
}

void UTouchWidget::NativeDestruct()
{
	Super::NativeDestruct();
	RemoveTouchDelegate(WidgetTouchComponent);
}


void UTouchWidget::SetIsEnabled(bool bInIsEnabled)
{
	Super::SetIsEnabled(bInIsEnabled);
	SetVisibleDisabled(!GetIsEnabled(), true);
}

UUserWidget* UTouchWidget::GetParentUserWidget()
{
	return ParentUserWidget;
}

void UTouchWidget::SetParentUserWidget(UUserWidget* InUserWidget)
{
	ParentUserWidget = InUserWidget;
}

void UTouchWidget::BindTouchDelegate()
{
	if (WidgetTouchComponent)
	{
		RemoveTouchDelegate(WidgetTouchComponent);
	}
	if (GetWidgetTouchComponent())
	{
		if (TriggerIndex != 255)
		{
			WidgetTouchComponent->AddObjectTouchs(this, TriggerIndex);
		}
		WidgetTouchComponent->DelegateBind(10, true, this, TEXT("NativeTouchIndexLocation"));
		if (!WidgetTouchComponent->OnComponentDeactivated.IsAlreadyBound(this, &UTouchWidget::ComponentDeactivated))
		{
			WidgetTouchComponent->OnComponentDeactivated.AddDynamic(this, &UTouchWidget::ComponentDeactivated);
		}
		return;
	}
	if (GetWorld())
	{
		FLatentActionManager& LatentActionManager = GetWorld()->GetLatentActionManager();
		FLatentActionInfo Latentinfo;
		Latentinfo.CallbackTarget = this;
		Latentinfo.ExecutionFunction = TEXT("BindTouchDelegate");
		Latentinfo.Linkage = 0;
		Latentinfo.UUID = UKismetMathLibrary::RandomIntegerInRange(0, 222);
		LatentActionManager.AddNewAction(this, Latentinfo.UUID, new FDelayAction(0.2, Latentinfo));
	}
}

void UTouchWidget::RemoveTouchDelegate(UTouchComponent* TouchComponent)
{
	if (TouchComponent)
	{
		if (TriggerIndex != 255)
		{
			TouchComponent->RemoveObjectTouchs(this);
		}
		TouchComponent->DelegateBind(10, false, this, TEXT("NativeTouchIndexLocation"));
		if (TouchComponent->OnComponentDeactivated.IsAlreadyBound(this, &UTouchWidget::ComponentDeactivated))
		{
			TouchComponent->OnComponentDeactivated.RemoveDynamic(this, &UTouchWidget::ComponentDeactivated);
		}
		if (TouchComponent == WidgetTouchComponent)
		{
			WidgetTouchComponent = nullptr;
		}
	}
}

void UTouchWidget::NativeTouchIndexLocation(const FVector& Location, uint8 FingerIndex)
{
	if (Location.Z == 0.0f || TriggerIndex == 255)
	{
		TouchIndexLocation(Location, FingerIndex);
	}
}

bool UTouchWidget::TouchIndexLocation(const FVector& Location, uint8 FingerIndex)
{
	if (!GetIsEnabled() || GetVisibility() != ESlateVisibility::Visible)  /** * 是否启用,只有可视才能互交 */
	{
		return false;
	}
	if (IsTouchLocation(Location))
	{
		LastTriggerLocation = Location;
		LastTriggerLocation.Z = FingerIndex;
		OnTouchLocation.Broadcast(LastTriggerLocation); /** * 触发触摸位置 */
		TriggerInedxAnimation(0);
	}
	return true;
}



void UTouchWidget::SetIndexTouchDelegate(bool bDelegateBind, uint8 FingerIndex)
{
	if (GetWidgetTouchComponent())
	{
		WidgetTouchComponent->DelegateBind(FingerIndex, bDelegateBind, this, TEXT("TouchMovedLocation"));
	}
}

void UTouchWidget::TouchMovedLocation(const FVector& Location)
{
	if (!GetIsEnabled() || GetVisibility() != ESlateVisibility::Visible)  /** * 是否启用,只有可视才能互交 */
	{
		return;
	}
	LastTriggerLocation = Location;
	/** * 子类继承重写使用 */
}

FVector2D UTouchWidget::GetLocalPosition()
{
	FVector2D Offset = GetPaintSpaceGeometry().GetLocalPositionAtCoordinates({ 0.0,0.0 });
	UWidget* Parent = GetParent();
	while (Parent)
	{
		Offset += Parent->GetPaintSpaceGeometry().GetLocalPositionAtCoordinates({ 0.0,0.0 });
		Parent = Parent->GetParent();
	}
	if (ParentUserWidget)
	{
		Parent = ParentUserWidget;
		while (Parent)
		{
			Offset += Parent->GetPaintSpaceGeometry().GetLocalPositionAtCoordinates({ 0.0,0.0 });
			Parent = Parent->GetParent();
		}
	}
	return Offset + CustomOffsetPosition;
}

/** * 判断是否触控位置是否进入触控区域 */

bool UTouchWidget::IsTouchLocation(const FVector& Location)
{
	float ViewportScale = UWidgetLayoutLibrary::GetViewportScale(this); /** * 视口触控缩放 */
	FVector2D SizeLocation = GetPaintSpaceGeometry().GetLocalSize() * ViewportScale * GetRenderTransform().Scale; /** * 获取控件大小 */
	LocalWidgetPosition = GetLocalPosition(); /** * 获取控件左上角位置 */
	LocalCentreWidgetPosition = LocalWidgetPosition + SizeLocation / 2;
	TriggerOffsetPosition = FVector2D(Location) / ViewportScale - LocalWidgetPosition;
	FVector2D TLocalWidgetPosition = LocalWidgetPosition * ViewportScale - SizeLocation / 4 * (GetRenderTransform().Scale - 1); /** * 计算缩放偏移 */
	return Location.X >= TLocalWidgetPosition.X && Location.X <= TLocalWidgetPosition.X + SizeLocation.X  \
		&& Location.Y >= TLocalWidgetPosition.Y && Location.Y <= TLocalWidgetPosition.Y + SizeLocation.Y; // \是链接下一行 后面不许有空格
}

void UTouchWidget::SetVisibleDisabled(bool bVisible, bool bFlushInput)
{
}


void UTouchWidget::TriggerInedxAnimation(int Index)
{
	BPTriggerInedxAnimation(Index);
}

void UTouchWidget::ComponentDeactivated(UActorComponent* ActorComponent)
{
	if (ActorComponent)
	{
		UTouchComponent* TouchComponent = Cast<UTouchComponent>(ActorComponent);
		if (TouchComponent)
		{
			RemoveTouchDelegate(TouchComponent);
		}
	}
}

void UTouchWidget::SetTriggerIndex(uint8 Index)
{
	if (GetOwningPlayer())
	{
		UActorComponent* ActorComponent = GetOwningPlayer()->GetComponentByClass(UTouchComponent::StaticClass());
		if (ActorComponent)
		{
			UTouchComponent* TouchComponent = Cast<UTouchComponent>(ActorComponent);
			if (TouchComponent)
			{
				if (TriggerIndex != 255)
				{
					TouchComponent->RemoveObjectTouchs(this);
				}
				if (Index != 255)
				{
					TouchComponent->AddObjectTouchs(this, TriggerIndex);
				}
				TriggerIndex = Index;
			}
		}
	}
}

UTouchComponent* UTouchWidget::GetWidgetTouchComponent()
{
	if (WidgetTouchComponent)
	{
		return WidgetTouchComponent;
	}
	if (GetOwningPlayer())
	{
		UActorComponent* ActorComponent = GetOwningPlayer()->GetComponentByClass(UTouchComponent::StaticClass());
		if (ActorComponent)
		{
			WidgetTouchComponent = Cast<UTouchComponent>(ActorComponent);
		}
	}
	return WidgetTouchComponent;
}


