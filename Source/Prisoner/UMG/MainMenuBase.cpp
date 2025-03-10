// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenuBase.h"

void UMainMenuBase::NativeConstruct()
{
	Super::NativeConstruct();

	mStartBtn = Cast<UButton>(GetWidgetFromName(TEXT("StartBtn")));
	mEndBtn = Cast<UButton>(GetWidgetFromName(TEXT("EndBtn")));


	mStartBtn->OnClicked.AddDynamic(this, &UMainMenuBase::OnStartBtnClicked);
	mStartBtn->OnHovered.AddDynamic(this, &UMainMenuBase::OnStartBtnHover);
	mStartBtn->OnUnhovered.AddDynamic(this, &UMainMenuBase::OnStartBtnUnhover);
}

void UMainMenuBase::NativeTick(const FGeometry& _geo, float _DeltaTime)
{
	Super::NativeTick(_geo, _DeltaTime);
}

void UMainMenuBase::OnStartBtnClicked()
{
	if (!mNextLevelName.IsNone())
	{
		UGameplayStatics::OpenLevel(GetWorld(), mNextLevelName);
	}
}

void UMainMenuBase::OnStartBtnHover()
{
}

void UMainMenuBase::OnStartBtnUnhover()
{
}
