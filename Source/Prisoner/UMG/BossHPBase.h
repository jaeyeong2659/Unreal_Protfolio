// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "Blueprint/UserWidget.h"

#include <Components/ProgressBar.h>
#include "BossHPBase.generated.h"

/**
 * 
 */
UCLASS()
class PRISONER_API UBossHPBase : public UUserWidget
{
	GENERATED_BODY()

private:
	UProgressBar* m_HPBar;

public:
	void SetHP(float _Ratio);

public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& _geo, float _DeltaTime) override;
	
};
