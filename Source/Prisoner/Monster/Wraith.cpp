// Fill out your copyright notice in the Description page of Project Settings.


#include "Wraith.h"
#include "MonsterAIController.h"
#include "../Player/PlayerCharacter.h"
#include "../Particle/ParticleCascade.h"

AWraith::AWraith()
{
	PrimaryActorTick.bCanEverTick = true;

	mMonsterTableRowName = FName("Wraith");

	GetCapsuleComponent()->SetCapsuleHalfHeight(88.f);
	GetCapsuleComponent()->SetCapsuleRadius(34.f);

	GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -88.f));
	GetMesh()->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));



}

void AWraith::BeginPlay()
{
	Super::BeginPlay();

	mDissolveTimeMax = 1.41f;

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

void AWraith::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
}

void AWraith::UnPossessed()
{
	Super::UnPossessed();
}

void AWraith::Attack()
{
	//�ѱ� ��ġ ����
	FVector MuzzleLoc = GetMesh()->GetSocketLocation(TEXT("Muzzle_01"));

	//������ �����ǵ��� ����
	FActorSpawnParameters SpawnParam;
	SpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	//�ѱ� �߻� ����Ʈ ����
	AParticleCascade* MuzzleParticle = GetWorld()->SpawnActor<AParticleCascade>(MuzzleLoc,
		GetActorForwardVector().Rotation(), SpawnParam);

	//����Ʈ ����
	MuzzleParticle->SetParticle(mInfo.Effect.AttackEffect);


	// ������ �߻�� ��ġ ����
	FVector End = MuzzleLoc + GetActorForwardVector() * mInfo.AttackDistance;

	//�ڽ��� �浹 ���� 
	FCollisionQueryParams param(NAME_None, false, this);

	//�Ѹ� �±� ������ TArray(X)
	FHitResult result;


	//Trace �߻� 
	bool Hit = GetWorld()->LineTraceSingleByChannel(result, MuzzleLoc, End, ECollisionChannel::ECC_GameTraceChannel6, param);

#if ENABLE_DRAW_DEBUG
	FColor Color = Hit ? FColor::Red : FColor::Green;

	DrawDebugLine(GetWorld(), MuzzleLoc, End, Color, false, 1.f, 2.f);
#endif // ENABLE_DRAW_DEBUG

	//�÷��̾ �¾Ҵٸ�?
	if (Hit)
	{
		AAIController* MonsterController = Cast<AAIController>(GetController());
		APlayerCharacter* Target = Cast<APlayerCharacter>(MonsterController->GetBlackboardComponent()->GetValueAsObject(TEXT("Target")));

		FActorSpawnParameters HitSpawnParam;
		HitSpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		AParticleCascade* Particle = GetWorld()->SpawnActor<AParticleCascade>(result.ImpactPoint,
			result.ImpactNormal.Rotation(), HitSpawnParam);

		Particle->SetParticle(mInfo.Effect.HitPointEffect);

		Target->DamageTake(mInfo.Info.AttackPoint);
	}


}
void AWraith::Spawn()
{
	FActorSpawnParameters SpawnParam;
	SpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	AParticleCascade* Particle = GetWorld()->SpawnActor<AParticleCascade>(GetActorLocation(), GetActorRotation(), SpawnParam);

	Particle->SetParticle(mInfo.Effect.SpawnEffect);
	Particle->SetLifeSpan(1.f);
}

void AWraith::DeathEvent()
{
	FActorSpawnParameters SpawnParam;
	SpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	AParticleCascade* Particle = GetWorld()->SpawnActor<AParticleCascade>(GetActorLocation(), GetActorRotation(), SpawnParam);

	Particle->SetParticle(mInfo.Effect.DeathEffect);

	FAttachmentTransformRules AttachRules(EAttachmentRule::SnapToTarget, true);
	Particle->AttachToComponent(GetMesh(), AttachRules);
	Particle->SetLifeSpan(1.f);
}