// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "GameInfo.h"
#include "Player/PlayerCharacter.h"


#include "Blueprint/UserWidget.h"
#include "UMG/MainHUDBase.h"
#include "UMG/GameClearUIBase.h"


#include "GameFramework/GameModeBase.h"
#include "PrisonerGameModeBase.generated.h"



UCLASS()
class PRISONER_API APrisonerGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	TArray<TSubclassOf<APlayerCharacter>> m_PlayerClassArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	TArray<class AMonsterSpawnPoint*> mSpawnPointArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	int32 mRoundDeathMonsterCount;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	int32 mTargetMonsterCount;

	TSubclassOf<UUserWidget> m_MainHUDClass;
	UMainHUDBase* m_MainHUD;

	TSubclassOf<UUserWidget> m_GameClearClass;
	UGameClearUIBase* m_GameClearBase;

	ERoundType mCrrentRound;
	ERoundType mRoundStatus;

	bool LevelSequenceCheck;
	ALevelSequenceActor* mLevelSequence;
public:
	APrisonerGameModeBase();

public:
	virtual void BeginPlay() override;
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void Tick(float DeltaTime)	override;

public:
	void SetLevelSequenceCheck(bool _Set)
	{
		LevelSequenceCheck = _Set;
	}
	bool GetLevelSequenceCheck() const
	{
		return LevelSequenceCheck;
	}
	void SetLevelSequence(ALevelSequenceActor* _Sequence)
	{
		mLevelSequence = _Sequence;
	}

	UMainHUDBase* GetMain()
	{
		return m_MainHUD;
	}

	void AddSpawnPointArray(class AMonsterSpawnPoint* _SpawnPoint)
	{
		mSpawnPointArray.Add(_SpawnPoint);
		//PrintViewport(3.f, FColor::Red, TEXT("Add!"));
	}


	void AddMonsterDeathCount()
	{
		mRoundDeathMonsterCount++;
	}
	int32 GetMonsterDeathCount() const
	{
		return mRoundDeathMonsterCount;
	}

	void SetMainHUDVisibility(bool _Set);

	void RoundSet();
	void RoundStart();
	void RoundClear();
	void EvolutionCheck();

	void SetMission();

	void BossHPVisibility(bool _Set);
	void BossHPSet(float _Ratio);


	void VisibleGameClearHUD();

	
};
