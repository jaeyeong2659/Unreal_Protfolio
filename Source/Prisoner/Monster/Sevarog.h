// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Monster.h"
#include "Sevarog.generated.h"

/**
 * 
 */
UCLASS()
class PRISONER_API ASevarog : public AMonster
{
	GENERATED_BODY()

public:
	ASevarog();

protected:
	class UMonsterAnimInstance* mAnimInst;

public:
	virtual void OnConstruction(const FTransform& Transform);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	virtual void PossessedBy(AController* NewController);
	virtual void UnPossessed();

public:
	virtual float DamageTake(float DamageAmount) override;
	virtual void Attack();
	virtual void Skill1();
	virtual void Skill2();
	virtual void Skill3();
	virtual void Spawn() override;
	virtual void DeathEvent() override;
};
