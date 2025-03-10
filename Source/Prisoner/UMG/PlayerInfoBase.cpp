// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerInfoBase.h"

void UPlayerInfoBase::SetHP(float _Ratio)
{
	m_HPBar->SetPercent(_Ratio);
}

void UPlayerInfoBase::SetMP(float _Ratio)
{
	m_MPBar->SetPercent(_Ratio);
}

void UPlayerInfoBase::NativeConstruct()
{
	Super::NativeConstruct();

	m_HPBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("HP_Bar")));
	m_MPBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("MP_Bar")));
}

void UPlayerInfoBase::NativeTick(const FGeometry& _geo, float _DeltaTime)
{
	Super::NativeTick(_geo, _DeltaTime);
}
