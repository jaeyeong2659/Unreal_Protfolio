// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameInfo.h"
#include "Engine/GameInstance.h"
#include "PrisonerGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class PRISONER_API UPrisonerGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UPrisonerGameInstance();
	~UPrisonerGameInstance();

private:
	EPlayerJob	m_SelectJob;
	UDataTable* m_SelectJobDesTable;
	UDataTable* m_PlayerInfo;
	UDataTable* m_MonsterInfo;
	UDataTable* m_LevelUpTable;
	UDataTable* m_MonsterSkillInfoTable;

	ETechType mEvolutionTech;
	FPlayerInfo mInfo;

	ERoundType m_RoundType;

	FVector CharacterLocation;
	FRotator CharacterRotation;

	bool BossMission;
public:
	void SetBossMission(bool _Set)
	{
		BossMission = _Set;
	}
	bool GetBossMission() const
	{
		return BossMission;
	}
	

	void SetCharacterLocation(FVector _Location)
	{
		CharacterLocation = _Location;
	}

	FVector GetCharacterLocation() const
	{
		return CharacterLocation;
	}

	void SetRoundType(ERoundType _Type)
	{
		m_RoundType = _Type;
	}

	ERoundType GetRoundType() const
	{
		return m_RoundType;
	}


	ETechType GetEvolutionTech() const
	{
		return mEvolutionTech;
	}

	void SetEvolutionTech(ETechType _Tech)
	{
		mEvolutionTech = _Tech;
	}

	void SetPlayerInfo(FPlayerInfo& _Info)
	{
		mInfo = _Info;
	}

	FPlayerInfo GetPlayerInfo() const
	{
		return mInfo;
	}


public:
	void SetSelectJob(EPlayerJob Job)
	{
		m_SelectJob = Job;
	}
	EPlayerJob GetSelectJob() const
	{
		return m_SelectJob;
	}

public:
	const FSelectJobDes* FindSelectJobTable(const FName& Name);
	const FPlayerInfo* FindPlayerInfoTable(const FName& Name);
	const FMonsterInfo* FindMonsterInfoTable(const FName& Name);
	const FLevelUpTable* FindLevelUpTable(const FName& Name);
	const FSkillData* FindMonsterSkillTable(const FName& Name);
};
