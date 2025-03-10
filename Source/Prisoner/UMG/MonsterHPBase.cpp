// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterHPBase.h"



void UMonsterHPBase::NativeConstruct()
{
	Super::NativeConstruct();

	m_HPBar = Cast<UProgressBar>(GetWidgetFromName(FName(TEXT("HP_Bar"))));
	m_HPBar->SetPercent(m_HPRatio);
}

void UMonsterHPBase::NativeTick(const FGeometry& _geo, float _DeltaTime)
{
	Super::NativeTick(_geo, _DeltaTime);
}

void UMonsterHPBase::SetHP(float _Ratio)
{
	if (IsValid(m_HPBar))
	{
		m_HPBar->SetPercent(_Ratio);
	}
}

