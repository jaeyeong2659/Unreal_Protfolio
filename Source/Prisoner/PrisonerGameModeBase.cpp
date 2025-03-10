// Copyright Epic Games, Inc. All Rights Reserved.


#include "PrisonerGameModeBase.h"
#include "PrisonerGameInstance.h"
#include "Player/PrisonerPlayerController.h"
#include "Player/KnightCharacter.h"
#include "Player/SoldierCharacter.h"
#include "Player/MinionCharacter.h"
#include "Monster/MonsterSpawnPoint.h"
#include "Trigger/CinematicTrigger.h"
#include "Monster/Sevarog.h"



APrisonerGameModeBase::APrisonerGameModeBase()
{
	PrimaryActorTick.bCanEverTick = true;

	PlayerControllerClass = APrisonerPlayerController::StaticClass();

	//mEvolutionTech=ETechType::Tech_1;

	ConstructorHelpers::FClassFinder<UUserWidget> Finder(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Blueprints/UMG/UI_MainHUD.UI_MainHUD_C'"));
	if (Finder.Succeeded())
	{
		m_MainHUDClass = Finder.Class;
	}

	ConstructorHelpers::FClassFinder<UUserWidget> GameClearFinder(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Blueprints/UMG/UI_GameClear.UI_GameClear_C'"));
	if (Finder.Succeeded())
	{
		m_GameClearClass = GameClearFinder.Class;
	}


	mRoundDeathMonsterCount = 0;

	mCrrentRound = ERoundType::None;
	mRoundStatus = ERoundType::Round_Clear;

	mTargetMonsterCount = 10;


}

void APrisonerGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	if (IsValid(m_MainHUDClass))
	{
		m_MainHUD = Cast<UMainHUDBase>(CreateWidget(GetWorld(), m_MainHUDClass));
		if (IsValid(m_MainHUD))
		{
			m_MainHUD->AddToViewport();
			m_MainHUD->SetHP(1.f);
			m_MainHUD->SetMP(1.f);

			FString CurrentLevelName = UGameplayStatics::GetCurrentLevelName(this);
			if(CurrentLevelName == FString(TEXT("Boss")))
				SetMainHUDVisibility(false);
		}
	}

	
	/*TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMonsterSpawnPoint::StaticClass(), FoundActors);

	for (AActor* Actor : FoundActors)
	{
		AMonsterSpawnPoint* SpawnPoint = Cast<AMonsterSpawnPoint>(Actor);

		if (SpawnPoint)
		{
			mSpawnPointArray.Add(SpawnPoint);
			PrintViewport(3.f, FColor::Red, TEXT("h~"));
		}
		PrintViewport(3.f, FColor::Red, FString::Printf(TEXT("PointArray : %d"),mSpawnPointArray.Num()));
	}*/

	LevelSequenceCheck = false;


}

void APrisonerGameModeBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	UPrisonerGameInstance* GameInst = Cast<UPrisonerGameInstance>(GetWorld()->GetGameInstance());

	if (IsValid(GameInst))
	{
		EPlayerJob PlayerIndexType = GameInst->GetSelectJob();
		int32 PlayerIndex = 0;

		switch (PlayerIndexType)
		{
		case EPlayerJob::Knight:
			PlayerIndex = 1;
			break;
		case EPlayerJob::Soldier:
			PlayerIndex = 2;
			break;
		case EPlayerJob::Minion:
			PlayerIndex = 0;
			break;
		case EPlayerJob::Narbash:
			PlayerIndex = 3;
			break;
		case EPlayerJob::Revenant:
			PlayerIndex = 4;
			break;
		default:
			PlayerIndex = 0;
			break;
		}



		if (PlayerIndex)
		{
			DefaultPawnClass = m_PlayerClassArray[PlayerIndex];
		}
	}

	RoundSet();
}

void APrisonerGameModeBase::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
}

void APrisonerGameModeBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SetMission();
	if (mRoundDeathMonsterCount >= mTargetMonsterCount)
	{
		RoundClear();
	}

}

void APrisonerGameModeBase::SetMainHUDVisibility(bool _Set)
{
	if (IsValid(m_MainHUD))
	{
		if (_Set)
			m_MainHUD->SetVisibility(ESlateVisibility::Visible);
		else
			m_MainHUD->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void APrisonerGameModeBase::RoundSet()
{
	int32 Count = mSpawnPointArray.Num();

	for (int32 i = 0; i < Count; i++)
	{
		mSpawnPointArray[i]->AllRemoveMonster();
	}

	UPrisonerGameInstance* GameInst = Cast<UPrisonerGameInstance>(GetWorld()->GetGameInstance());


	if (IsValid(GameInst))
	{
		mCrrentRound = GameInst->GetRoundType();
		if (mCrrentRound == ERoundType::None)
		{
			mCrrentRound = ERoundType::One_Round;
			mTargetMonsterCount = 5;
		}
		else if (mCrrentRound == ERoundType::One_Round)
		{
			mCrrentRound = ERoundType::Two_Round;
			mTargetMonsterCount = 8;


		}
		else if (mCrrentRound == ERoundType::Two_Round)
		{
			mCrrentRound = ERoundType::Three_Round;
			mTargetMonsterCount = 10;

		}
		else if (mCrrentRound == ERoundType::Three_Round)
		{
			mCrrentRound = ERoundType::One_Round;
			mTargetMonsterCount = 10;
		}

			GameInst->SetRoundType(mCrrentRound);
	}
	


	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &APrisonerGameModeBase::RoundStart, 1.f, false);
}

void APrisonerGameModeBase::RoundStart()
{

	mRoundStatus = ERoundType::Round_isPlay;
	int32 Count = mSpawnPointArray.Num();

	for (int32 i = 0; i < Count; i++)
	{
		mSpawnPointArray[i]->SetSpawnLock(false);
	}

}

void APrisonerGameModeBase::RoundClear()
{
	if (mCrrentRound != ERoundType::Three_Round)
		mRoundDeathMonsterCount = 0;
	mRoundStatus = ERoundType::Round_Clear;
	int32 Count = mSpawnPointArray.Num();

	for (int32 i = 0; i < Count; i++)
	{
		mSpawnPointArray[i]->AllRemoveMonster();
	}

	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &APrisonerGameModeBase::EvolutionCheck, 2.f, false);
}

void APrisonerGameModeBase::EvolutionCheck()
{
	UPrisonerGameInstance* GameInst = Cast<UPrisonerGameInstance>(GetWorld()->GetGameInstance());

	if (IsValid(GameInst))
	{
		ETechType Type = GameInst->GetEvolutionTech();

		if (Type != ETechType::Tech_3)
		{
			APlayerCharacter* Player = GetWorld()->GetFirstPlayerController()->GetPawn<APlayerCharacter>();

			if (IsValid(Player))
			{
				Player->Evolution();
			}
		}
	}
}

void APrisonerGameModeBase::SetMission()
{
	int32 MonsterCount = mTargetMonsterCount - mRoundDeathMonsterCount;

	MonsterCount <= 0 ? 0 : MonsterCount;

	UPrisonerGameInstance* GameInst = Cast<UPrisonerGameInstance>(GetWorld()->GetGameInstance());

	if (IsValid(GameInst))
	{
		ERoundType RoundType = GameInst->GetRoundType();

		if (IsValid(m_MainHUD))
		{

			if (RoundType == ERoundType::Three_Round && mRoundDeathMonsterCount >= mTargetMonsterCount)
			{
				m_MainHUD->LastMission();
			}
			else
				m_MainHUD->SetCountAndRound(MonsterCount, RoundType);


		}
	}

	
}

void APrisonerGameModeBase::BossHPVisibility(bool _Set)
{
	if (IsValid(m_MainHUD))
	{
		if (_Set)
			m_MainHUD->VisibilityBossHP(true);
		else
			m_MainHUD->VisibilityBossHP(false);
	}
}

void APrisonerGameModeBase::BossHPSet(float _Ratio)
{
	if (IsValid(m_MainHUD))
	{
		m_MainHUD->SetBossHP(_Ratio);
	}
}

void APrisonerGameModeBase::VisibleGameClearHUD()
{
	if (IsValid(m_GameClearClass))
	{
		m_GameClearBase = Cast<UGameClearUIBase>(CreateWidget(GetWorld(), m_GameClearClass));
		if (IsValid(m_GameClearBase))
		{
			m_GameClearBase->AddToViewport();
			m_GameClearBase->SetDesText(GetWorld()->GetTimeSeconds());
		}
	}
}

