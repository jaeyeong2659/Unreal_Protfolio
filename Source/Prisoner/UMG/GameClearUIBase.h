// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include <Components/TextBlock.h>
#include "Blueprint/UserWidget.h"
#include "GameClearUIBase.generated.h"

/**
 * 
 */
UCLASS()
class PRISONER_API UGameClearUIBase : public UUserWidget
{
	GENERATED_BODY()

private:
	UTextBlock* mTime;

public:
	void SetDesText(float _Time);

public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
};
