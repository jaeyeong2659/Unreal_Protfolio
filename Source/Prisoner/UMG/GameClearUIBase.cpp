// Fill out your copyright notice in the Description page of Project Settings.


#include "GameClearUIBase.h"

void UGameClearUIBase::SetDesText(float _Time)
{
	//mTime->SetText(FText::FromString(Des.ToString().Replace(TEXT("\\n"), TEXT("\n"))));
	mTime->SetText(FText::FromString(FString::Printf(TEXT("소요 시간 \n %d초"), (int32)_Time)));
}

void UGameClearUIBase::NativeConstruct()
{
	Super::NativeConstruct();

	mTime = Cast<UTextBlock>(GetWidgetFromName(FName(TEXT("Required_Time"))));
}

void UGameClearUIBase::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}
