// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameInfo.h"
#include "UMG/SelectWidget.h"
#include "Evolution/EvolutionCharacter.h"
#include "Evolution/EvolutionCharacterTwo.h"
#include "GameFramework/GameModeBase.h"
#include "EvolutionGameMode.generated.h"

/**
 * 
 */
UCLASS()
class PRISONER_API AEvolutionGameMode : public AGameModeBase
{
	GENERATED_BODY()

private:
	TSubclassOf<USelectWidget> m_SelectHUDClass;
	USelectWidget* m_SelectHUD;

	AEvolutionCharacter* mEvolutionCharacter;
	AEvolutionCharacterTwo* mEvolutionCharacterTwo;

public:
	USelectWidget* GetSelectWidget()
	{
		return m_SelectHUD;
	}

	void SetEvolutionCharacter(AEvolutionCharacter* _Character)
	{
		mEvolutionCharacter = _Character;
	}

	void SetEvolutionCharacter(AEvolutionCharacterTwo* _Character)
	{
		mEvolutionCharacterTwo = _Character;
	}

	AEvolutionCharacter* GetEvolutionCharacter() const
	{
		return mEvolutionCharacter;
	}
	
	AEvolutionCharacterTwo* GetEvolutionCharacterTwo() const
	{
		return mEvolutionCharacterTwo;
	}
public:
	AEvolutionGameMode();

public:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTIme) override;
};
