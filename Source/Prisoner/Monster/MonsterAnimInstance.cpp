// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterAnimInstance.h"
#include "Monster.h"

UMonsterAnimInstance::UMonsterAnimInstance()
{
	mType = EMonsterAnimType::Spawn;
	mHitAdditive = 0.f;
}

void UMonsterAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
}

void UMonsterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
}

void UMonsterAnimInstance::AnimNotify_Attack()
{
	AMonster* Monster = Cast<AMonster>(TryGetPawnOwner());

	if (IsValid(Monster))
		Monster->Attack();
}

void UMonsterAnimInstance::AnimNotify_AttackEnable()
{
}

void UMonsterAnimInstance::AnimNotify_AttackEnd()
{
	AMonster* Monster = Cast<AMonster>(TryGetPawnOwner());
	if (IsValid(Monster))
		Monster->SetAttackEnd(true);
}

void UMonsterAnimInstance::AnimNotify_HitEnd()
{
	mHitAdditive = 0.f;
}

void UMonsterAnimInstance::AnimNotify_DeathEnd()
{
	AMonster* Monster = Cast<AMonster>(TryGetPawnOwner());

	if (IsValid(Monster))
	{
		Monster->OnDissolve();
		//Monster->OnRagdoll();
	}
}

void UMonsterAnimInstance::AnimNotify_Spawn()
{
	AMonster* Monster = Cast<AMonster>(TryGetPawnOwner());

	if (IsValid(Monster))
	{
		Monster->Spawn();
		Monster->OnSpawnDissolve();
	}
}

void UMonsterAnimInstance::AnimNotify_SpawnEnd()
{
	mType = EMonsterAnimType::Idle;
}

void UMonsterAnimInstance::AnimNotify_Skill1()
{
	AMonster* Monster = Cast<AMonster>(TryGetPawnOwner());

	if (IsValid(Monster))
	{
		Monster->Skill1();
	}
}

void UMonsterAnimInstance::AnimNotify_Skill2()
{
	AMonster* Monster = Cast<AMonster>(TryGetPawnOwner());

	if (IsValid(Monster))
	{
		Monster->Skill2();
	}
}

void UMonsterAnimInstance::AnimNotify_Skill3()
{
	AMonster* Monster = Cast<AMonster>(TryGetPawnOwner());

	if (IsValid(Monster))
	{
		Monster->Skill3();
	}
}

void UMonsterAnimInstance::AnimNotify_SkillEnd()
{
	AMonster* Monster = Cast<AMonster>(TryGetPawnOwner());

	if (IsValid(Monster))
	{
		Monster->ClearCurrentSkill();
	}
}
