// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "Blueprint/UserWidget.h"

#include "PlayerInfoBase.h"
#include "BossHPBase.h"
#include "Components/TextBlock.h"

#include "MainHUDBase.generated.h"

/**
 * 
 */
UCLASS()
class PRISONER_API UMainHUDBase : public UUserWidget
{
	GENERATED_BODY()
	

private:
	UPlayerInfoBase* m_PlayerInfo;
	UTextBlock* m_MissionTextBlock;
	UBossHPBase* m_BossHPBase;

	int32 mMonsterCount;
	int32 mRound;

	bool MissionLock;
	

public:
	void SetHP(float _Ratio);
	void SetMP(float _Ratio);
	void SetBossHP(float _Ratio);
	void SetCountAndRound(int32 _MonsterCount, ERoundType _CurrentRound);
	void LastMission();
	void VisibilityBossHP(bool _Set);

public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& _geo, float _DeltaTime) override;

};
