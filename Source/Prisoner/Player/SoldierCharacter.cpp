// Fill out your copyright notice in the Description page of Project Settings.


#include "SoldierCharacter.h"
#include "../PrisonerGameInstance.h"
#include "../Particle/ParticleCascade.h"

ASoldierCharacter::ASoldierCharacter()
{
	GetMesh()->SetRelativeLocation(FVector(0.0, 0.0, -88.0));
	GetMesh()->SetRelativeRotation(FRotator(0.0, -90, 0.0));

	mPlayerInfoTableName = FName("Soldier");
}

void ASoldierCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void ASoldierCharacter::Tick(float DeltaTIme)
{
	Super::Tick(DeltaTIme);
}

void ASoldierCharacter::NomalAttackCheck()
{
	FVector MuzzleLoc = GetMesh()->GetSocketLocation(TEXT("Muzzle_Front"));

	FVector StartLocation = GetActorLocation() + GetActorForwardVector() * 30.f;
	FVector EndLocation = StartLocation + GetActorForwardVector() * mInfo.AttackDistance;


	FCollisionQueryParams param(NAME_None, false, this);

	FHitResult CollisionResult;

	bool CollisionEnable = GetWorld()->LineTraceSingleByChannel(CollisionResult, StartLocation, EndLocation, ECC_GameTraceChannel2, param);
	
#if ENABLE_DRAW_DEBUG

	FColor DrawColor = CollisionEnable ? FColor::Red : FColor::Green;

	DrawDebugLine(GetWorld(), StartLocation, EndLocation, DrawColor, false, 1.f, 2.f);
#endif	

	if (CollisionEnable)
	{
		FActorSpawnParameters SpawnParam;
		SpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		AParticleCascade* MuzzleParticle = GetWorld()->SpawnActor<AParticleCascade>(MuzzleLoc, GetActorForwardVector().Rotation(), SpawnParam);
		AParticleCascade* Particle = GetWorld()->SpawnActor<AParticleCascade>(CollisionResult.ImpactPoint, CollisionResult.ImpactNormal.Rotation(), SpawnParam);

		MuzzleParticle->SetParticle(mInfo.Effect.AttackEffect);
		Particle->SetParticle(mInfo.Effect.HitPointEffect);

		AMonster* Monster = Cast<AMonster>(CollisionResult.GetActor());

		Monster->DamageTake(mInfo.Info.AttackPoint);
	}
}
