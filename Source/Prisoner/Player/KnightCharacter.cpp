// Fill out your copyright notice in the Description page of Project Settings.


#include "KnightCharacter.h"
#include "../PrisonerGameInstance.h"
#include "../Particle/ParticleCascade.h"

AKnightCharacter::AKnightCharacter() 
{
	/*static ConstructorHelpers::FObjectFinder<USkeletalMesh> KnightAsset(TEXT("/Script/Engine.SkeletalMesh'/Game/ParagonMinions/Characters/Minions/Dusk_Minions/Meshes/Minion_Lane_Melee_Dusk.Minion_Lane_Melee_Dusk'"));
	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimClass(TEXT("/Script/Engine.AnimBlueprint'/Game/Player/Animation/AB_Knight.AB_Knight_C'"));


	GetMesh()->SetSkeletalMesh(KnightAsset.Object);
	GetMesh()->SetAnimInstanceClass(AnimClass.Class);*/

	GetMesh()->SetRelativeLocation(FVector(0.0, 0.0, -88.0));
	GetMesh()->SetRelativeRotation(FRotator(0.0, -90, 0.0));

	mPlayerInfoTableName = FName("Knight");
}



void AKnightCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AKnightCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AKnightCharacter::NomalAttackCheck()
{
	FVector StartLocation = GetActorLocation() + GetActorForwardVector() * 30.f;
	FVector EndLocation = StartLocation + GetActorForwardVector() * mInfo.AttackDistance;

	FCollisionQueryParams param(NAME_None, false, this);

	TArray<FHitResult>	CollisionResult;
	bool CollisionEnable = GetWorld()->SweepMultiByChannel(CollisionResult, StartLocation, EndLocation, FQuat::Identity,
		ECC_GameTraceChannel2, FCollisionShape::MakeSphere(50.f), param);

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

			AMonster* Monster = Cast<AMonster>(CollisionResult[i].GetActor());

			if (IsValid(Monster))
				Monster->DamageTake(mInfo.Info.AttackPoint);
		}
	}


}
