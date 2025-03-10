// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <Components/ProgressBar.h>
#include "../GameInfo.h"
#include "Blueprint/UserWidget.h"
#include "MonsterHPBase.generated.h"

/**
 * 
 */
UCLASS()
class PRISONER_API UMonsterHPBase : public UUserWidget
{
	GENERATED_BODY()

protected:
	UProgressBar* m_HPBar;
	float m_HPRatio;

public:
	void SetInitHP(float _Ratio)
	{
		m_HPRatio = _Ratio;
	}
	
	void SetHP(float _Ratio);

public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& _geo, float _DeltaTime) override;

};
