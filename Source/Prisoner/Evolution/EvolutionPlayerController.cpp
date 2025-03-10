// Fill out your copyright notice in the Description page of Project Settings.


#include "EvolutionPlayerController.h"

AEvolutionPlayerController::AEvolutionPlayerController()
{
	PrimaryActorTick.bCanEverTick = true;
	bShowMouseCursor = true;

	// ���콺 ���� �ִ� ���͸� �����ϴ� ��� ��
	bEnableMouseOverEvents = true;

	bEnableClickEvents = true;
	bEnableTouchEvents = true;
	bEnableTouchOverEvents = true;
}

void AEvolutionPlayerController::BeginPlay()
{
	Super::BeginPlay();

	FInputModeGameAndUI Mode;
	SetInputMode(Mode);
}

void AEvolutionPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AEvolutionPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
}
