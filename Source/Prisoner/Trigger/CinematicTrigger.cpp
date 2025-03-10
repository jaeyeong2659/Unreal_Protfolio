// Fill out your copyright notice in the Description page of Project Settings.


#include "CinematicTrigger.h"
#include "../PrisonerGameModeBase.h"

// Sets default values
ACinematicTrigger::ACinematicTrigger()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	mSequencePlayer = nullptr;
	mSequenceActor = nullptr;
}

void ACinematicTrigger::TriggerBegin(const FHitResult& SweepResult)
{
	if (IsValid(mLevelSequence))
	{
		if (!mSequencePlayer)
		{
			mSequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(
				GetWorld(), mLevelSequence, mSetting, mSequenceActor);


			APrisonerGameModeBase* GameMode = GetWorld()->GetAuthGameMode<APrisonerGameModeBase>();

			if (IsValid(GameMode))
			{
				GameMode->SetLevelSequence(mSequenceActor);
			}
		}



		


		/*
		����� ���۵ɶ�
		FOnMovieSceneSequencePlayerEvent	OnPlay;

		����� �ɶ�
		FOnMovieSceneSequencePlayerEvent	OnPlayReverse;

		����ϴ� �������� ���⶧
		FOnMovieSceneSequencePlayerEvent	OnStop;

		����ϴ� �������� �Ͻ����� �ɶ�
		FOnMovieSceneSequencePlayerEvent	OnPause;

		����ϴ� �������� ������
		FOnMovieSceneSequencePlayerEvent	OnFinished;
		*/

		mSequencePlayer->Play();

		mSequencePlayer->OnFinished.AddDynamic(this, &ACinematicTrigger::SequenceFinish);
	}
}

void ACinematicTrigger::TriggerEnd()
{
}

void ACinematicTrigger::SequenceFinish()
{
}
