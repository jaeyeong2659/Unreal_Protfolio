// Fill out your copyright notice in the Description page of Project Settings.


#include "PrisonerPlayerController.h"

APrisonerPlayerController::APrisonerPlayerController()
{
	PrimaryActorTick.bCanEverTick = true;

	GrenadePick = false;
}

void APrisonerPlayerController::BeginPlay()
{
	Super::BeginPlay();

	FInputModeGameAndUI Mode;
	SetInputMode(Mode);
}

void APrisonerPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APrisonerPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
}

void APrisonerPlayerController::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void APrisonerPlayerController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);
}

void APrisonerPlayerController::OnUnPossess()
{
	Super::OnUnPossess();
}
