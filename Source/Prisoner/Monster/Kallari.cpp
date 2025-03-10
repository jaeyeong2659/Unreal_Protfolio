// Fill out your copyright notice in the Description page of Project Settings.


#include "Kallari.h"
#include "MonsterAIController.h"
#include "../Player/PlayerCharacter.h"
#include "../Particle/ParticleCascade.h"


AKallari::AKallari()
{
	PrimaryActorTick.bCanEverTick = true;

	mMonsterTableRowName = FName("Kallari");

	GetCapsuleComponent()->SetCapsuleHalfHeight(88.f);
	GetCapsuleComponent()->SetCapsuleRadius(34.f);

	GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -88.f));
	GetMesh()->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));



}

void AKallari::BeginPlay()
{
	Super::BeginPlay();

	mDissolveTimeMax = 1.79f;

	int32 NumMaterials = 0;
	TArray<USkeletalMeshComponent*> MeshComponents;
	GetComponents<USkeletalMeshComponent>(MeshComponents);

	for (USkeletalMeshComponent* MeshComp : MeshComponents)
	{
		NumMaterials += MeshComp->GetNumMaterials();
	}

	for (int32 i = 0; i < NumMaterials; i++)
	{
		UMaterialInstanceDynamic* Mtrl = GetMesh()->CreateDynamicMaterialInstance(i, nullptr);
		mDissolveMtrlArray.Add(Mtrl);
	}
	

}

void AKallari::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
}

void AKallari::UnPossessed()
{
	Super::UnPossessed();
}

void AKallari::Attack()
{
	AAIController* MonsterController = Cast<AAIController>(GetController());

	APlayerCharacter* Target = Cast<APlayerCharacter>(MonsterController->GetBlackboardComponent()->GetValueAsObject(TEXT("Target")));

	if (IsValid(Target))
	{
		FActorSpawnParameters SpawnParam;
		SpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		FVector Dir = GetActorLocation() - Target->GetActorLocation();
		Dir.Normalize();

		FVector ParticleLoc = Target->GetActorLocation() + Dir * 50.f;

		AParticleCascade* Particle = GetWorld()->SpawnActor<AParticleCascade>(ParticleLoc, Dir.Rotation(), SpawnParam);

		//Particle->SetParticle(TEXT("/Script/Engine.ParticleSystem'/Game/ParagonMinions/FX/Particles/Minions/Minion_melee/FX/Impacts/P_Minion_Impact_Default.P_Minion_Impact_Default'"));
		Particle->SetParticle(mInfo.Effect.AttackEffect);

		//Target->TakeDamage((float)mInfo.Info.AttackPoint, FDamageEvent(), GetController(), this);

		Target->DamageTake(mInfo.Info.AttackPoint);

	}
}
void AKallari::Spawn()
{
	FActorSpawnParameters SpawnParam;
	SpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	AParticleCascade* Particle = GetWorld()->SpawnActor<AParticleCascade>(GetActorLocation(), GetActorRotation(), SpawnParam);

	Particle->SetParticle(mInfo.Effect.SpawnEffect);
	Particle->SetLifeSpan(1.f);
}

void AKallari::DeathEvent()
{
	FActorSpawnParameters SpawnParam;
	SpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	AParticleCascade* Particle = GetWorld()->SpawnActor<AParticleCascade>(GetActorLocation(), GetActorRotation(), SpawnParam);

	Particle->SetParticle(mInfo.Effect.DeathEffect);
	Particle->SetLifeSpan(1.f);

	FAttachmentTransformRules AttachRules(EAttachmentRule::SnapToTarget, true);
	Particle->AttachToComponent(GetMesh(), AttachRules);
}