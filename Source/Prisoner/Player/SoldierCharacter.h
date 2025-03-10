// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PlayerCharacter.h"
#include "SoldierCharacter.generated.h"

/**
 * 
 */
UCLASS()
class PRISONER_API ASoldierCharacter : public APlayerCharacter
{
	GENERATED_BODY()

public:
	ASoldierCharacter();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTIme) override;



public:
	virtual void NomalAttackCheck() override;
	
};
