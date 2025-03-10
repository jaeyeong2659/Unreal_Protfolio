// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "Animation/AnimInstance.h"
#include "EvolutionAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class PRISONER_API UEvolutionAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UEvolutionAnimInstance();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
		EEvolutionAnimType mAnimType;

public:
	void ChangeAnim(EEvolutionAnimType Anim)
	{
		mAnimType = Anim;
	}
	EEvolutionAnimType GetSelectAnimType() const
	{
		return mAnimType;
	}

public:
	virtual void NativeInitializeAnimation();
	virtual void NativeUpdateAnimation(float DeltaSeconds);


public:
	UFUNCTION()
		void AnimNotify_SelectEnd();
};
