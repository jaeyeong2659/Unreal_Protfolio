// Fill out your copyright notice in the Description page of Project Settings.


#include "SelectWidget.h"
#include "../PrisonerGameInstance.h"


void USelectWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// SelectBtn 지정 후 OnClicked으로 호버, 언호버 이벤트 델리게이트 등록.
	m_StartBtn = Cast<UButton>(GetWidgetFromName(FName(TEXT("StartBtn"))));
	m_NameBorder = Cast<UBorder>(GetWidgetFromName(FName(TEXT("NameBorder"))));
	m_DesBorder = Cast<UBorder>(GetWidgetFromName(FName(TEXT("DesBorder"))));
	m_JobName = Cast<UTextBlock>(GetWidgetFromName(FName(TEXT("JobName"))));
	m_JobDes = Cast<UTextBlock>(GetWidgetFromName(FName(TEXT("JobDsc"))));

	m_StartBtn->OnClicked.AddDynamic(this, &USelectWidget::OnStartBtnClicked);
}

void USelectWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void USelectWidget::OnStartBtnClicked()
{
	SetStartButtonVisibility(false);

	UPrisonerGameInstance* GameInst = Cast<UPrisonerGameInstance>(GetWorld()->GetGameInstance());
	
	if (IsValid(GameInst))
	{
		if (GameInst->GetEvolutionTech() == ETechType::Tech_1)
		{
			GameInst->SetEvolutionTech(ETechType::Tech_2);
		}
		else if(GameInst->GetEvolutionTech() == ETechType::Tech_2)
		{
			GameInst->SetEvolutionTech(ETechType::Tech_3);
		}
	}

	// 레벨 전환
	UGameplayStatics::OpenLevel(GetWorld(), FName(TEXT("Main")));

}

void USelectWidget::SetStartButtonVisibility(bool Visible)
{
	if (Visible)
	{
		m_StartBtn->SetVisibility(ESlateVisibility::Visible);
		m_NameBorder->SetVisibility(ESlateVisibility::Visible);
		m_DesBorder->SetVisibility(ESlateVisibility::Visible);
	}

	else
	{
		m_StartBtn->SetVisibility(ESlateVisibility::Collapsed);
		m_NameBorder->SetVisibility(ESlateVisibility::Collapsed);
		m_DesBorder->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void USelectWidget::SetDesText(FText& Name, FText& Des)
{

	m_JobName->SetText(Name);
	m_JobDes->SetText(FText::FromString(Des.ToString().Replace(TEXT("\\n"), TEXT("\n"))));
}
