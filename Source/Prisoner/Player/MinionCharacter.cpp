// Fill out your copyright notice in the Description page of Project Settings.

#include "MinionCharacter.h"
#include "../PrisonerGameInstance.h"
#include "../Particle/ParticleCascade.h"
#include "Engine/EngineTypes.h"

AMinionCharacter::AMinionCharacter()
{
	GetMesh()->SetRelativeLocation(FVector(0.0, 0.0, -88.0));
	GetMesh()->SetRelativeRotation(FRotator(0.0, -90, 0.0));

	mPlayerInfoTableName = FName("Minion");
}



void AMinionCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AMinionCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMinionCharacter::NomalAttackCheck()
{

	//PrintViewport(1.f, FColor::Red, TEXT("hehe"));
	FVector StartLocation = GetActorLocation() + GetActorForwardVector() * 30.f;
	FVector EndLocation = StartLocation + GetActorForwardVector() * mInfo.AttackDistance;

	FCollisionQueryParams param(NAME_None, false, this);

	TArray<FHitResult>	CollisionResult;
	bool CollisionEnable = GetWorld()->SweepMultiByChannel(CollisionResult, StartLocation, EndLocation, FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel2, FCollisionShape::MakeSphere(50.f), param);

#if ENABLE_DRAW_DEBUG

	FColor DrawColor = CollisionEnable ? FColor::Red : FColor::Green;

	DrawDebugCapsule(GetWorld(), (StartLocation + EndLocation) / 2, mInfo.AttackDistance / 2, 50.f, FRotationMatrix::MakeFromZ(GetActorForwardVector()).ToQuat(),
		DrawColor, false, 0.5f);

#endif	


	if (CollisionEnable)
	{
		int32 Count = CollisionResult.Num();

		for (int32 i = 0; i < Count; ++i)
		{
			FActorSpawnParameters SpawnParam;
			SpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

			AParticleCascade* Particle = GetWorld()->SpawnActor<AParticleCascade>(CollisionResult[i].ImpactPoint,
				CollisionResult[i].ImpactNormal.Rotation(), SpawnParam);


			Particle->SetParticle(mInfo.Effect.AttackEffect);

			//FDamageEvent DamageEvent();
			//FPointDamageEvent DamageEvent((float)mInfo.Info.AttackPoint, CollisionResult[i], CollisionResult[i].ImpactNormal, this);
			//float DamageA = mInfo.Info.AttackPoint;

			//FPointDamageEvent DamageEvent()
			//using FDamageEvent = TSubclassOf<UDamageType>;

			//FPointDamageEvent DamageEvent((float)mInfo.Info.AttackPoint, CollisionResult[i], CollisionResult[i].ImpactNormal, nullptr);
			//CollisionResult[i].GetActor()->TakeDamage((float)mInfo.Info.AttackPoint, DamageEvent(), GetController(), this);
			//FDamageEvent DamageEvent((float)mInfo.Info.AttackPoint, CollisionResult[i], CollisionResult[i].ImpactNormal, nullptr);
			//FDamageEvent DamageEvent(FPointDamageEvent(DamageA, CollisionResult[i], CollisionResult[i].ImpactNormal, nullptr));
			//CollisionResult[i].GetActor()->TakeDamage(DamageA, DamageEvent, GetController(), this);

			AMonster* Monster = Cast<AMonster>(CollisionResult[i].GetActor());

			if(IsValid(Monster))
				Monster->DamageTake(mInfo.Info.AttackPoint);
		}
	}
}