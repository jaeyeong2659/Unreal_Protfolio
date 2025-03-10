// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameInfo.h"

#include "Blueprint/UserWidget.h"
#include "UMG/MainMenuBase.h"

#include "GameFramework/GameModeBase.h"
#include "IntroGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class PRISONER_API AIntroGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	AIntroGameModeBase();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
		ULevelSequence* mLevelSequence;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
		FMovieSceneSequencePlaybackSettings	mSetting;

	ULevelSequencePlayer* mSequencePlayer;
	ALevelSequenceActor* mSequenceActor;


	TSubclassOf<UUserWidget> mMainMenuClass;
	UMainMenuBase* mMainMenu;


public:
	virtual void BeginPlay() override;
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void Tick(float DeltaTime)	override;
	void Play();

	
};
