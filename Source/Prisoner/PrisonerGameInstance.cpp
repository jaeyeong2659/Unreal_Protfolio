// Fill out your copyright notice in the Description page of Project Settings.


#include "PrisonerGameInstance.h"

UPrisonerGameInstance::UPrisonerGameInstance() :
	m_SelectJob(EPlayerJob::None),
	m_SelectJobDesTable(nullptr),
	m_PlayerInfo(nullptr),
	m_MonsterInfo(nullptr),
	m_LevelUpTable(nullptr),
	m_MonsterSkillInfoTable(nullptr)
{
	static ConstructorHelpers::FObjectFinder<UDataTable>	SelectJobDes(TEXT("/Script/Engine.DataTable'/Game/DataTable/SelectJobDes.SelectJobDes'"));
	if (SelectJobDes.Succeeded())
		m_SelectJobDesTable = SelectJobDes.Object;

	
	static ConstructorHelpers::FObjectFinder<UDataTable> PlayerInfo(TEXT("/Script/Engine.DataTable'/Game/DataTable/PlayerInfo.PlayerInfo'"));
	if (PlayerInfo.Succeeded())
		m_PlayerInfo = PlayerInfo.Object;

	static ConstructorHelpers::FObjectFinder<UDataTable> MonsterInfo(TEXT("/Script/Engine.DataTable'/Game/DataTable/MonsterInfo.MonsterInfo'"));
	if (MonsterInfo.Succeeded())
		m_MonsterInfo = MonsterInfo.Object;

	static ConstructorHelpers::FObjectFinder<UDataTable> LevelUpTable(TEXT("/Script/Engine.DataTable'/Game/DataTable/LevelUpTable.LevelUpTable'"));
	if (MonsterInfo.Succeeded())
		m_LevelUpTable = LevelUpTable.Object;

	static ConstructorHelpers::FObjectFinder<UDataTable> MonsterSkillInfo(TEXT("/Script/Engine.DataTable'/Game/DataTable/MonsterSkillTable.MonsterSkillTable'"));
	if (MonsterInfo.Succeeded())
		m_MonsterSkillInfoTable = MonsterSkillInfo.Object;

	mEvolutionTech = ETechType::Tech_1;
	m_SelectJob = EPlayerJob::None;

	m_RoundType = ERoundType::None;


	CharacterLocation = FVector::ZeroVector;
	CharacterRotation = FRotator::ZeroRotator;
	BossMission;

}

UPrisonerGameInstance::~UPrisonerGameInstance()
{
}

const FSelectJobDes* UPrisonerGameInstance::FindSelectJobTable(const FName& Name)
{
	return m_SelectJobDesTable->FindRow<FSelectJobDes>(Name, TEXT(""));
}

const FPlayerInfo* UPrisonerGameInstance::FindPlayerInfoTable(const FName& Name)
{
	return m_PlayerInfo->FindRow<FPlayerInfo>(Name, TEXT(""));
}

const FMonsterInfo* UPrisonerGameInstance::FindMonsterInfoTable(const FName& Name)
{
	return m_MonsterInfo->FindRow<FMonsterInfo>(Name, TEXT(""));
}

const FLevelUpTable* UPrisonerGameInstance::FindLevelUpTable(const FName& Name)
{
	return m_LevelUpTable->FindRow<FLevelUpTable>(Name, TEXT(""));
}

const FSkillData* UPrisonerGameInstance::FindMonsterSkillTable(const FName& Name)
{
	return m_MonsterSkillInfoTable->FindRow<FSkillData>(Name, TEXT(""));
}
