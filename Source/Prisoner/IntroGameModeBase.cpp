// Fill out your copyright notice in the Description page of Project Settings.


#include "IntroGameModeBase.h"

AIntroGameModeBase::AIntroGameModeBase()
{
	mSequencePlayer = nullptr;
	mSequenceActor = nullptr;


	ConstructorHelpers::FClassFinder<UUserWidget> Finder(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Blueprints/UMG/UI_MainMenuHUD.UI_MainMenuHUD_C'"));
	if (Finder.Succeeded())
	{
		mMainMenuClass = Finder.Class;
	}
}

void AIntroGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	if (IsValid(mMainMenuClass))
	{
		mMainMenu = Cast<UMainMenuBase>(CreateWidget(GetWorld(), mMainMenuClass));
		if (IsValid(mMainMenu))
		{
			mMainMenu->AddToViewport();
		}
	}
}

void AIntroGameModeBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	Play();
}

void AIntroGameModeBase::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
}

void AIntroGameModeBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AIntroGameModeBase::Play()
{
	if (IsValid(mLevelSequence))
	{
		if (!IsValid(mSequencePlayer))
		{
			mSequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(GetWorld(), mLevelSequence, mSetting, mSequenceActor);
		}


		mSequencePlayer->PlayLooping();

		mSequencePlayer->OnFinished.AddDynamic(this, &AIntroGameModeBase::Play);
	}
}
