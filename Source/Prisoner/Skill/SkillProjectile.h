// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SkillActor.h"
#include "SkillProjectile.generated.h"

/**
 * 
 */
UCLASS()
class PRISONER_API ASkillProjectile : public ASkillActor
{
	GENERATED_BODY()


public:
	ASkillProjectile();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
		UProjectileMovementComponent* mProjectile;


	float	mDistance;
	FVector	mPrevLocation;


public:
	UProjectileMovementComponent* GetProjectile() const
	{
		return mProjectile;
	}

	void SetDistance(float _Distance)
	{
		mDistance = _Distance;
	}

protected:
	virtual void BeginPlay() override;


public:
	virtual void Tick(float DeltaTime) override;

public:
	UFUNCTION()
		void CollisionProjectile(const FHitResult& Hit);

	
};
