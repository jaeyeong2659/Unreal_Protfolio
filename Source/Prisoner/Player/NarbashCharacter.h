// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PlayerCharacter.h"
#include "NarbashCharacter.generated.h"

/**
 * 
 */
UCLASS()
class PRISONER_API ANarbashCharacter : public APlayerCharacter
{
	GENERATED_BODY()

public:
	ANarbashCharacter();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;


public:
	virtual void NomalAttackCheck() override;
	
};
