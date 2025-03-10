// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerAnimInstance.h"
#include "PlayerCharacter.h"

UPlayerAnimInstance::UPlayerAnimInstance() : 
	mAnimType(EPlayerAnimType::Ground)
{
	mMoveDir = 0.f;
	mGround = true;

	mAttack = false;
	mAttackEnable = true;
	mAttackIndex = 0;

	mAnimSet = false;

	mUseSkillNumber = -1;

}

void UPlayerAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
}

void UPlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(TryGetPawnOwner());

	if (IsValid(PlayerCharacter))
	{
		UCharacterMovementComponent* Movement = PlayerCharacter->GetCharacterMovement();

		mSpeedRatio = Movement->Velocity.Size() / Movement->MaxWalkSpeed;

		mGround = Movement->IsMovingOnGround();
	}
}

void UPlayerAnimInstance::Attack()
{
	//PrintViewport(1.f, FColor::Red, TEXT("WTF"));

	if (!mAttackEnable)
		return;
	//PrintViewport(1.f, FColor::Red, TEXT("WTF2"));

	mAttackEnable = false;

	if (!Montage_IsPlaying(mAttackMontageArray[mAttackIndex]))
	{
		Montage_SetPosition(mAttackMontageArray[mAttackIndex], 0.f);
		Montage_Play(mAttackMontageArray[mAttackIndex]);

		mAttackIndex = (mAttackIndex + 1) % mAttackMontageArray.Num();
		mAttack = true;
	}

	else
		mAttackEnable = true;
}

void UPlayerAnimInstance::Hit()
{
	if (!IsValid(mHitMontage))
		return;

	mAttackEnable = true;
	mHitAdditive = 1.f;

	if (!Montage_IsPlaying(mHitMontage))
	{
		Montage_SetPosition(mHitMontage, 0.f);
		Montage_Play(mHitMontage);
	}
		
}

void UPlayerAnimInstance::UseSkill(int32 SkillNumber)
{
	int32 Count = mSkillMontageArray.Num();

	for (int32 i = 0; i < Count; i++)
	{
		if (mSkillMontageArray[i].SkillNumber == SkillNumber)
		{
			mUseSkillNumber = SkillNumber;
			
			if (!Montage_IsPlaying(mSkillMontageArray[i].Montage))
			{
				Montage_SetPosition(mSkillMontageArray[i].Montage,0.f);
				Montage_Play(mSkillMontageArray[i].Montage);
			}

			break;
		}
	}
}

void UPlayerAnimInstance::AnimNotify_Attack()
{
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(TryGetPawnOwner());

	if (IsValid(PlayerCharacter))
	{
		PlayerCharacter->NomalAttackCheck();
	}

}

void UPlayerAnimInstance::AnimNotify_AttackEnable()
{
	mAttackEnable = true;
}

void UPlayerAnimInstance::AnimNotify_AttackEnd()
{
	mAttackIndex = 0;
	mAttackEnable = true;
	mAttack = false;
}

void UPlayerAnimInstance::AnimNotify_DeathEnd()
{

}

void UPlayerAnimInstance::AnimNotify_Spawn()
{
	APlayerCharacter* Player = Cast<APlayerCharacter>(TryGetPawnOwner());

	if (IsValid(Player))
	{
		Player->SetDisableInput(true);
	}
}

void UPlayerAnimInstance::AnimNotify_SpawnEnd()
{
	APlayerCharacter* Player = Cast<APlayerCharacter>(TryGetPawnOwner());

	if (IsValid(Player))
	{
		Player->SetDisableInput(false);
	}

	ChangePlayerAnimType(EPlayerAnimType::Ground);
}

void UPlayerAnimInstance::AnimNotify_HitEnd()
{
	mHitAdditive = 0.f;
}

void UPlayerAnimInstance::AnimNotify_UseSkill()
{
	if (mUseSkillNumber == -1)
		return;

	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(TryGetPawnOwner());

	if (IsValid(PlayerCharacter))
		PlayerCharacter->UseSkill(mUseSkillNumber);
}
