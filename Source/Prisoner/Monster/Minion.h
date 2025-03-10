// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Monster.h"
#include "Minion.generated.h"

/**
 * 
 */
UCLASS()
class PRISONER_API AMinion : public AMonster
{
	GENERATED_BODY()

public:
	AMinion();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	virtual void PossessedBy(AController* NewController) override;
	virtual void UnPossessed() override;

public:
	virtual void Attack() override;
	virtual void Spawn() override;
	virtual void DeathEvent() override;
};