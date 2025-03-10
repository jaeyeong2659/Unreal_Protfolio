// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "Animation/AnimInstance.h"
#include "PlayerAnimInstance.generated.h"



USTRUCT(BlueprintType)
struct FSkillAnimationInfo
{
	GENERATED_USTRUCT_BODY()
		
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
		int32	SkillNumber;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
		UAnimMontage* Montage;
};

UCLASS()
class PRISONER_API UPlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UPlayerAnimInstance();

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	float mSpeedRatio;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	float mMoveDir;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	EPlayerAnimType mAnimType;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	bool mGround;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
		UAnimMontage* mHitMontage;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
		float mHitAdditive;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
		TArray<UAnimMontage*> mAttackMontageArray;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
		bool mAttackEnable;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
		int32 mAttackIndex;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
		bool mAttack;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
		float mAoPitch;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
		bool mAnimSet;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
		TArray<FSkillAnimationInfo> mSkillMontageArray;

	int32 mUseSkillNumber;

public:
	EPlayerAnimType GetPlayerAnimType() const
	{
		return mAnimType;
	}
	void ChangePlayerAnimType(EPlayerAnimType _Type)
	{
		mAnimType = _Type;
	}
	void SetMoveDir(float Dir)
	{
		mMoveDir = Dir;
	}
	void SetAimPitch(float _Pitch)
	{
		mAoPitch = _Pitch;
	}
	float GetAnimPitct() const
	{
		return  mAoPitch;
	}
	void SetAimSet(bool _On)
	{
		mAnimSet = _On;
	}
	bool GetAimSet() const
	{
		return mAnimSet;
	}

public:
	virtual void NativeInitializeAnimation();
	virtual void NativeUpdateAnimation(float DeltaSeconds);
	

public:
	void Attack();
	void Hit();
	void UseSkill(int32 SkillNumber);

public:
	UFUNCTION()
		void AnimNotify_Attack();
	UFUNCTION()
		void AnimNotify_AttackEnable();
	UFUNCTION()
		void AnimNotify_AttackEnd();
	UFUNCTION()
		void AnimNotify_DeathEnd();
	UFUNCTION()
		void AnimNotify_Spawn();
	UFUNCTION()
		void AnimNotify_SpawnEnd();
	UFUNCTION()
		void AnimNotify_HitEnd();
	UFUNCTION()
		void AnimNotify_UseSkill();
};
