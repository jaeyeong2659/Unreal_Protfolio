// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include <Components/Image.h>
#include "Blueprint/UserWidget.h"
#include "CrosshairWidgetBase.generated.h"

/**
 * 
 */
UCLASS()
class PRISONER_API UCrosshairWidgetBase : public UUserWidget
{
	GENERATED_BODY()

protected:
	UImage* m_Image;

public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& _geo, float _DeltaTime) override;
	
};
