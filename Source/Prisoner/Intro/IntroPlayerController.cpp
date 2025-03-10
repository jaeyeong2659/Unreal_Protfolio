// Fill out your copyright notice in the Description page of Project Settings.


#include "IntroPlayerController.h"

AIntroPlayerController::AIntroPlayerController()
{
	PrimaryActorTick.bCanEverTick = true;
	bShowMouseCursor = true;
}

void AIntroPlayerController::BeginPlay()
{
	Super::BeginPlay();

	FInputModeUIOnly Mode;
	SetInputMode(Mode);
}

void AIntroPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AIntroPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
}
