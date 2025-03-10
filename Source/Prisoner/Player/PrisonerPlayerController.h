// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "GameFramework/PlayerController.h"
#include "PrisonerPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class PRISONER_API APrisonerPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	APrisonerPlayerController();


protected:
	bool GrenadePick;

public:
	void SetGrenadePick(bool _Set)
	{
		GrenadePick = _Set;
	}
	bool GetGrenadePick() const
	{
		return GrenadePick;
	}

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	virtual void SetupInputComponent() ;
	virtual void PostInitializeComponents() override;

protected:
	virtual void OnPossess(APawn* aPawn) override;
	virtual void OnUnPossess() override;

	
};
