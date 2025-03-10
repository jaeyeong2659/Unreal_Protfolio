// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "Monster.h"
#include "Kallari.generated.h"

/**
 * 
 */
UCLASS()
class PRISONER_API AKallari : public AMonster
{
	GENERATED_BODY()
	
public:
	AKallari();

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
