// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "GameFramework/PlayerController.h"
#include "EvolutionPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class PRISONER_API AEvolutionPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AEvolutionPlayerController();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	virtual void SetupInputComponent();
	
};
