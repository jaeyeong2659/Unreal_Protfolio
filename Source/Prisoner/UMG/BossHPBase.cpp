// Fill out your copyright notice in the Description page of Project Settings.


#include "BossHPBase.h"

void UBossHPBase::SetHP(float _Ratio)
{
	m_HPBar->SetPercent(_Ratio);
}


void UBossHPBase::NativeConstruct()
{
	Super::NativeConstruct();

	m_HPBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("HP_Bar")));
}

void UBossHPBase::NativeTick(const FGeometry& _geo, float _DeltaTime)
{
	Super::NativeTick(_geo, _DeltaTime);
}
