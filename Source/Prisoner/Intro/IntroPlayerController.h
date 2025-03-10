// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "GameFramework/PlayerController.h"
#include "IntroPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class PRISONER_API AIntroPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	AIntroPlayerController();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	virtual void SetupInputComponent();
};
