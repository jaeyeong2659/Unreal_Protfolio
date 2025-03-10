// Fill out your copyright notice in the Description page of Project Settings.


#include "EvolutionAnimInstance.h"

UEvolutionAnimInstance::UEvolutionAnimInstance()
{
	mAnimType = EEvolutionAnimType::Walk;
}

void UEvolutionAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
}

void UEvolutionAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
}

void UEvolutionAnimInstance::AnimNotify_SelectEnd()
{
	ChangeAnim(EEvolutionAnimType::Idle);
}
