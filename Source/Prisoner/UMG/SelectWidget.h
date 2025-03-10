// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include <Components/Button.h>
#include <Components/Border.h>
#include <Components/TextBlock.h>
#include "Blueprint/UserWidget.h"
#include "SelectWidget.generated.h"

/**
 * Select Character Level UMG Base 
 */
UCLASS()
class PRISONER_API USelectWidget : public UUserWidget
{
	GENERATED_BODY()

private:
	UButton* m_StartBtn;
	UBorder* m_NameBorder;
	UBorder* m_DesBorder;
	UTextBlock* m_JobName;
	UTextBlock* m_JobDes;

public:
	void SetStartButtonVisibility(bool Visible);
	void SetDesText(FText &Name, FText &Des);

public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
	UFUNCTION()
		void OnStartBtnClicked();
};
