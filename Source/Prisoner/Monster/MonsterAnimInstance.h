// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "Animation/AnimInstance.h"
#include "MonsterAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class PRISONER_API UMonsterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UMonsterAnimInstance();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
		EMonsterAnimType mType;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
		float mHitAdditive;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
		UAnimMontage* mHitMontage;

public:
	void ChangeAnim(EMonsterAnimType Type)
	{
		mType = Type;
	}
	void Hit()
	{
		if (!IsValid(mHitMontage))
			return;

		mHitAdditive = 1.f;

		if (!Montage_IsPlaying(mHitMontage))
		{
			Montage_SetPosition(mHitMontage, 0.f);
			Montage_Play(mHitMontage);
		}
	}
	EMonsterAnimType GetAnimType() const
	{
		return mType;
	}


public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;


public:
	UFUNCTION()
		void AnimNotify_Attack();
	UFUNCTION()
		void AnimNotify_AttackEnable();
	UFUNCTION()
		void AnimNotify_AttackEnd();
	UFUNCTION()
		void AnimNotify_HitEnd();
	UFUNCTION()
		void AnimNotify_DeathEnd();
	UFUNCTION()
		void AnimNotify_Spawn();
	UFUNCTION()
		void AnimNotify_SpawnEnd();

	UFUNCTION()
		void AnimNotify_Skill1();

	UFUNCTION()
		void AnimNotify_Skill2();

	UFUNCTION()
		void AnimNotify_Skill3();

	UFUNCTION()
		void AnimNotify_SkillEnd();
};


	
