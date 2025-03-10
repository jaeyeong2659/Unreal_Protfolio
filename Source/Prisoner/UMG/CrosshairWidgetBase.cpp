// Fill out your copyright notice in the Description page of Project Settings.


#include "CrosshairWidgetBase.h"

void UCrosshairWidgetBase::NativeConstruct()
{
	Super::NativeConstruct();

	m_Image = Cast<UImage>(GetWidgetFromName(FName(TEXT("CrossHair"))));
}

void UCrosshairWidgetBase::NativeTick(const FGeometry& _geo, float _DeltaTime)
{
	Super::NativeTick(_geo, _DeltaTime);
}
