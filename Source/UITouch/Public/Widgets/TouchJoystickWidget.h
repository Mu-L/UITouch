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

#pragma once

#include "CoreMinimal.h"
#include "TouchWidget.h"
#include "UMG/Public/Components/SizeBox.h"
#include "UMG/Public/Components/Image.h"
#include "UMG/Public/Components/CanvasPanel.h"

#include "TouchJoystickWidget.generated.h"

/**
 * 
 */
UCLASS()
class UITOUCH_API UTouchJoystickWidget : public UTouchWidget
{
	GENERATED_BODY()
	


public:

	/** 缩放控件变量 */
	UPROPERTY(BlueprintReadOnly, Meta = (BindWidget), Category = "Aimo|Variable")
		USizeBox* SizeBoxWidget;

	/** 包裹控件的视口 */
	UPROPERTY(BlueprintReadOnly, Meta = (BindWidget), Category = "Aimo|Variable")
		UCanvasPanel* CanvasPanelWidget;

	/**  操控杆背景图片 */
	UPROPERTY(BlueprintReadOnly, Meta = (BindWidget), Category = "Aimo|Variable")
		UImage* BackdropImageWidget;

	/** 操控杆背景图片设置 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Aimo|Variable")
		FSlateBrush BackdropSlateBrush;

	/** 操控杆图片变量 */
	UPROPERTY(BlueprintReadOnly, Meta = (BindWidget), Category = "Aimo|Variable")
		UImage* ControlImageWidget;

	/** 操控杆图片设置 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Aimo|Variable")
		FSlateBrush ControlSlateBrush;

	/** * 触控索引 */
	UPROPERTY(BlueprintReadWrite, Category = "Aimo|Variable")
		uint8 TouchFingerIndex = 255;

	/** * Y轴倍数 */
	UPROPERTY(BlueprintReadWrite, Category = "Aimo|Variable")
		float YShaftTimes = -1.0f;

	/** * 忽略值 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Aimo|Variable")
		FVector2D IgnoreNumerical;

	/** * 摇杆动画倍数 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Aimo|Variable")
		float JoystickAnimationRangeMultiple = 1.0f;

protected:

	virtual void NativePreConstruct() override;

	virtual void NativeConstruct() override;
	

public:

	virtual	void TouchIndex(FVector Moved, uint8 FingerIndex) override;

	virtual void TouchMoved(FVector Moved) override;

	/** * 设置操控杆位置 */
	UFUNCTION(BlueprintCallable, Category = "Aimo|Function")
		virtual void SetControlPosition(FVector2D Moved);

};
