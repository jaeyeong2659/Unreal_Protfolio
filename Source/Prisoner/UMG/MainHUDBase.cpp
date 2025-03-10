// Fill out your copyright notice in the Description page of Project Settings.


#include "MainHUDBase.h"
#include "../PrisonerGameInstance.h"
#include "../PrisonerGameModeBase.h"

void UMainHUDBase::SetHP(float _Ratio)
{
	m_PlayerInfo->SetHP(_Ratio);
}

void UMainHUDBase::SetMP(float _Ratio)
{
	m_PlayerInfo->SetMP(_Ratio);
}

void UMainHUDBase::SetBossHP(float _Ratio)
{
	m_BossHPBase->SetHP(_Ratio);
}

void UMainHUDBase::SetCountAndRound(int32 _MonsterCount, ERoundType _CurrentRound)
{
	mMonsterCount = _MonsterCount;
	if (_CurrentRound == ERoundType::One_Round)
		mRound = 1;
	else if (_CurrentRound == ERoundType::Two_Round)
		mRound = 2;
	else
		mRound = 3;

	UPrisonerGameInstance* GameInst = Cast<UPrisonerGameInstance>(GetWorld()->GetGameInstance());

	bool Check = false;
	if (IsValid(GameInst))
	{
		Check = GameInst->GetBossMission();
	}

	if (!Check)
	{
		if (_CurrentRound != ERoundType::Three_Round)
		{
			FString Round = FString::Printf(TEXT("%d"), mRound) + TEXT(" Round \n") + TEXT("보스 몬스터가 당신의 침입을 감지했다. 일정 몬스터터를 죽이고 진화를 하여 최종 보스를 처치하라. \n");
			FString Count = FString::Printf(TEXT("죽여야하는 몬스터 수 : %d \n목표 마릿수를 처치하면 모든 몬스터는 사라집니다."), mMonsterCount);
			FString MissionText = Round + Count;
			m_MissionTextBlock->SetText(FText::FromString(MissionText));
		}
		else if (!MissionLock)
		{
			FString Round = FString::Printf(TEXT("%d"), mRound) + TEXT(" Round \n") + TEXT("보스 몬스터가 당신의 침입을 감지했다. 일정 몬스터터를 죽이고 진화를 하여 최종 보스를 처치하라. \n");
			FString Count = FString::Printf(TEXT("죽여야하는 몬스터 수 : %d \n목표 마릿수를 처치하면 모든 몬스터는 사라집니다."), mMonsterCount);
			FString MissionText = Round + Count;
			m_MissionTextBlock->SetText(FText::FromString(MissionText));
		}
	}
	else
	{
		FString MissionText = FString(TEXT("최종보스를 처치하라."));
		m_MissionTextBlock->SetText(FText::FromString(MissionText));
	}

	
	
}

void UMainHUDBase::LastMission()
{
	MissionLock = true;
	FString Mission = TEXT("보스 몬스터의 포탈이 맵 끝자락에 열렸다.\n 해당 포탈을 통해 보스 스테이지로 입장하라.");
	m_MissionTextBlock->SetText(FText::FromString(Mission));
}

void UMainHUDBase::VisibilityBossHP(bool _Set)
{
	if(_Set)
		m_BossHPBase->SetVisibility(ESlateVisibility::Visible);
	else
		m_BossHPBase->SetVisibility(ESlateVisibility::Collapsed);

}

void UMainHUDBase::NativeConstruct()
{
	Super::NativeConstruct();

	m_PlayerInfo = Cast<UPlayerInfoBase>(GetWidgetFromName(TEXT("UI_PlayerInfo")));
	m_MissionTextBlock = Cast<UTextBlock>(GetWidgetFromName(TEXT("MissionText")));
	m_BossHPBase = Cast<UBossHPBase>(GetWidgetFromName(TEXT("UI_BossHP")));
	MissionLock = false;


	m_BossHPBase->SetVisibility(ESlateVisibility::Collapsed);
	m_BossHPBase->SetHP(1);
}

void UMainHUDBase::NativeTick(const FGeometry& _geo, float _DeltaTime)
{
	Super::NativeTick(_geo, _DeltaTime);
}
