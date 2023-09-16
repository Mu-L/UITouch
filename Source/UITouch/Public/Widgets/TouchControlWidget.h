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
#include "Components/SizeBox.h"
#include "Components/Image.h"

#include "TouchControlWidget.generated.h"

/**
 * 
 */
UCLASS()
class UITOUCH_API UTouchControlWidget : public UTouchWidget
{
	GENERATED_BODY()
	

public:

	/** 缩放控件变量 */
	UPROPERTY(BlueprintReadWrite, Meta = (BindWidget), Category = "Aimo|Variable")
		USizeBox* SizeBoxWidget;

	/** 图像变量 */
	UPROPERTY(BlueprintReadOnly, Meta = (BindWidget), Category = "Aimo|Variable")
		UImage* ImageWidget;

	/** 图片设置 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Aimo|Variable")
		FSlateBrush SlateBrush;

	/** * 触控索引组 */
	UPROPERTY(BlueprintReadWrite, Category = "Aimo|Variable")
		TArray<uint8> TouchFingerIndexs;

	/** * 触控索引位置组 */
	UPROPERTY(BlueprintReadWrite, Category = "Aimo|Variable")
		TArray<FVector2D> TouchLocations;


protected:

	virtual void NativePreConstruct() override;

public:

	virtual	void TouchIndex(FVector Moved, uint8 FingerIndex) override;

	virtual void TouchMoved(FVector Moved) override;

};
