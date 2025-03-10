// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include <Components/Button.h>
#include "Blueprint/UserWidget.h"
#include "MainMenuBase.generated.h"

/**
 * 
 */
UCLASS()
class PRISONER_API UMainMenuBase : public UUserWidget
{
	GENERATED_BODY()


private:
	UButton* mStartBtn;
	UButton* mEndBtn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
		FName	mNextLevelName;


public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& _geo, float _DeltaTime);


public:
	UFUNCTION()
		void OnStartBtnClicked();

	UFUNCTION()
		void OnStartBtnHover();

	UFUNCTION()
		void OnStartBtnUnhover();

	
};
