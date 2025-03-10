// Fill out your copyright notice in the Description page of Project Settings.


#include "SuperMinion.h"
#include "MonsterAIController.h"
#include "../Player/PlayerCharacter.h"
#include "../Particle/ParticleCascade.h"

ASuperMinion::ASuperMinion()
{
	PrimaryActorTick.bCanEverTick = true;

	mMonsterTableRowName = FName("SuperMinion");

	GetCapsuleComponent()->SetCapsuleHalfHeight(88.f);
	GetCapsuleComponent()->SetCapsuleRadius(34.f);

	GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -88.f));
	GetMesh()->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));

	FConvertMaterial	Mtrl;
	Mtrl.Index = 0;

	mDissolveMtrlIndexArray.Add(Mtrl);

}

void ASuperMinion::BeginPlay()
{
	Super::BeginPlay();

	mDissolveTimeMax = 0.83f;
}

void ASuperMinion::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
}

void ASuperMinion::UnPossessed()
{
	Super::UnPossessed();
}

void ASuperMinion::Attack()
{
	AAIController* MonsterController = Cast<AAIController>(GetController());

	APlayerCharacter* Target = Cast<APlayerCharacter>(MonsterController->GetBlackboardComponent()->GetValueAsObject(TEXT("Target")));

	if (IsValid(Target))
	{
		FActorSpawnParameters SpawnParam;
		SpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		FVector Dir = GetActorLocation() - Target->GetActorLocation();
		FRotator Rot = Dir.Rotation();
		Rot.Yaw += 90.f;
		Dir.Normalize();

		FVector ParticleLoc = Target->GetActorLocation() + Dir * 50.f;

		AParticleCascade* Particle = GetWorld()->SpawnActor<AParticleCascade>(ParticleLoc, Rot, SpawnParam);

		Particle->SetParticle(mInfo.Effect.AttackEffect);

		//Target->TakeDamage((float)mInfo.Info.AttackPoint, FDamageEvent(), GetController(), this);

		Target->DamageTake(mInfo.Info.AttackPoint);

	}
}

void ASuperMinion::Spawn()
{
	FActorSpawnParameters SpawnParam;
	SpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	AParticleCascade* Particle = GetWorld()->SpawnActor<AParticleCascade>(GetActorLocation(), GetActorRotation(), SpawnParam);

	Particle->SetParticle(mInfo.Effect.SpawnEffect);
	Particle->SetLifeSpan(1.f);
}

void ASuperMinion::DeathEvent()
{
	FActorSpawnParameters SpawnParam;
	SpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	AParticleCascade* Particle = GetWorld()->SpawnActor<AParticleCascade>(GetActorLocation(), GetActorRotation(), SpawnParam);

	Particle->SetParticle(mInfo.Effect.DeathEffect);

	FAttachmentTransformRules AttachRules(EAttachmentRule::SnapToTarget, true);
	Particle->AttachToComponent(GetMesh(), AttachRules);
	Particle->SetLifeSpan(1.f);
}