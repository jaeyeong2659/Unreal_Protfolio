// Fill out your copyright notice in the Description page of Project Settings.


#include "Sevarog.h"
#include "MonsterAIController.h"
#include "MonsterAnimInstance.h"
#include "../Particle/ParticleCascade.h"
#include "../Skill/SkillProjectile.h"
#include "../PrisonerGameInstance.h"
#include "../Player/PlayerCharacter.h"
#include "../PrisonerGameModeBase.h"

ASevarog::ASevarog()
{
	PrimaryActorTick.bCanEverTick = true;

	//캡슐 크기설정
	GetCapsuleComponent()->SetCapsuleHalfHeight(200.f);
	GetCapsuleComponent()->SetCapsuleRadius(80.f);


	//매쉬 위치 설정
	GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -200.f));
	GetMesh()->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));

	mMonsterTableRowName = TEXT("Sevarog");

	mSkillNameArray.Add(TEXT("SevarogSkill1"));
	mSkillNameArray.Add(TEXT("SevarogSkill2"));
	mSkillNameArray.Add(TEXT("SevarogSkill3"));

	
}

void ASevarog::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
}

void ASevarog::BeginPlay()
{
	Super::BeginPlay();

	mWidgetComponent->DestroyComponent();
	mWidgetComponent = nullptr;


	GetMesh()->SetRelativeScale3D(FVector(2.f, 2.f, 2.f));
	mDissolveTimeMax = 0.73f;

	//몬스터 스폰 시 Dissovle 동작 시간.
	mDissolveTimeMax = 1.79f;
	//해당 몬스터의 머터리얼 갯수 만큼 Dissolve에 사용할 Array에 등록
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

	mAnimInst = Cast<UMonsterAnimInstance>(GetMesh()->GetAnimInstance());
	mAnimInst->ChangeAnim(EMonsterAnimType::Idle);


	APrisonerGameModeBase* GameMode = GetWorld()->GetAuthGameMode<APrisonerGameModeBase>();
	
	if (IsValid(GameMode))
	{
		GameMode->SetMainHUDVisibility(true);
		GameMode->BossHPVisibility(true);
	}

	UPrisonerGameInstance* GameInst = Cast<UPrisonerGameInstance>(GetWorld()->GetGameInstance());
	
}

void ASevarog::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	AMonsterAIController* AI = Cast<AMonsterAIController>(NewController);

	if (IsValid(AI))
	{
		AI->SetBehaviorTree(TEXT("/Script/AIModule.BehaviorTree'/Game/Monster/AI/BTBoss.BTBoss'"));
		AI->SetBlackboard(TEXT("/Script/AIModule.BlackboardData'/Game/Monster/AI/BBMonster.BBMonster'"));
	}
}

void ASevarog::UnPossessed()
{
	Super::UnPossessed();
}

float ASevarog::DamageTake(float DamageAmount)
{
	Super::DamageTake(DamageAmount);

	APrisonerGameModeBase* GameMode = GetWorld()->GetAuthGameMode<APrisonerGameModeBase>();

	if (IsValid(GameMode))
	{
		GameMode->BossHPSet(mInfo.Info.HP / (float)mInfo.Info.HPMax);
	}
	return 0.0f;
}

void ASevarog::Attack()
{
	AAIController* MonsterController = Cast<AAIController>(GetController());

	if (!IsValid(MonsterController))
		return;

	APlayerCharacter* Target = Cast<APlayerCharacter>(MonsterController->GetBlackboardComponent()->GetValueAsObject(TEXT("Target")));
	//APlayerCharacter* Player = Cast<APlayerCharacter>(Target);
	if (IsValid(Target))
	{
		FActorSpawnParameters SpawnParam;
		SpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		FVector Dir = GetActorLocation() - Target->GetActorLocation();
		Dir.Normalize();

		FVector ParticleLoc = Target->GetActorLocation() + Dir * 50.f;

		AParticleCascade* Particle = GetWorld()->SpawnActor<AParticleCascade>(ParticleLoc, Dir.Rotation(), SpawnParam);

		Particle->SetParticle(mInfo.Effect.AttackEffect);
		Particle->SetLifeSpan(2.f);


		Target->DamageTake(mInfo.Info.AttackPoint);
	}

}

void ASevarog::Skill2()
{
	if (mUseSkillIndex == -1)
		return;

	AAIController* MonsterController = Cast<AAIController>(GetController());

	APlayerCharacter* Target = Cast<APlayerCharacter>(MonsterController->GetBlackboardComponent()->GetValueAsObject(TEXT("Target")));

	if (IsValid(Target))
	{
		FActorSpawnParameters	SpawnParam;
		SpawnParam.SpawnCollisionHandlingOverride =	ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		// 타겟과 몬스터 사이에 이펙트를 재생한다.
		FVector	Dir =  Target->GetActorLocation() - GetActorLocation();
		Dir.Normalize();

		FVector	ParticleLoc = Target->GetActorLocation() + Dir * 50.f;

		AParticleCascade* Particle = GetWorld()->SpawnActor<AParticleCascade>(ParticleLoc, Dir.Rotation(), SpawnParam);

		UParticleSystem* PlayEffect = nullptr;
		UParticleSystem* TrailEffect = nullptr;

		int32	EffectCount = mSkillDataArray[mUseSkillIndex].SkillEffectArray.Num();

		for (int32 i = 0; i < EffectCount; ++i)
		{
			switch (mSkillDataArray[mUseSkillIndex].SkillEffectArray[i].Type)
			{
			case ESkillEffectType::Cast:
				break;
			case ESkillEffectType::Play:
				PlayEffect = mSkillDataArray[mUseSkillIndex].SkillEffectArray[i].Effect;
				break;
			case ESkillEffectType::Trail:
				break;
			}

		}

		Particle->SetParticle(PlayEffect);

		float	DmgRatio = 0.f;

		int32	OptionCount = mSkillDataArray[mUseSkillIndex].SkillOptionArray.Num();

		for (int32 i = 0; i < OptionCount; ++i)
		{
			if (mSkillDataArray[mUseSkillIndex].SkillOptionArray[i].Type ==
				ESkillOptionType::Damage)
				DmgRatio += mSkillDataArray[mUseSkillIndex].SkillOptionArray[i].Option;
		}

		DmgRatio >= 0.f ? 1.f : DmgRatio;

		Target->DamageTake((float)mInfo.Info.AttackPoint * DmgRatio);
	}
}

void ASevarog::Skill1()
{
	if (mUseSkillIndex == -1)
		return;

	AAIController* MonsterController = Cast<AAIController>(GetController());

	APlayerCharacter* Target = Cast<APlayerCharacter>(MonsterController->GetBlackboardComponent()->GetValueAsObject(TEXT("Target")));

	if (IsValid(Target))
	{
		FActorSpawnParameters	SpawnParam;
		SpawnParam.SpawnCollisionHandlingOverride =	ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		FVector CastLoc = GetMesh()->GetSocketLocation(TEXT("Hand_L_Cast"));
		FVector ProjectileStartLoc = GetActorLocation(); // -FVector(0.f, 0.f, GetCapsuleComponent()->GetScaledCapsuleHalfHeight());

		FVector Dir = (Target->GetActorLocation() - ProjectileStartLoc).GetSafeNormal();
		//FVector TargetLoc = Target->GetActorLocation();
		//FVector Direction = TargetLoc - CastLoc;

		//FRotator TargetRotation = FRotationMatrix::MakeFromX(Direction).Rotator();

		ASkillProjectile*  Skill = GetWorld()->SpawnActor<ASkillProjectile>(ProjectileStartLoc, Dir.Rotation(), SpawnParam);

		UParticleSystem* PlayEffect = nullptr;
		UParticleSystem* TrailEffect = nullptr;
		
		int32	EffectCount = mSkillDataArray[mUseSkillIndex].SkillEffectArray.Num();

		for (int32 i = 0; i < EffectCount; ++i)
		{
			switch (mSkillDataArray[mUseSkillIndex].SkillEffectArray[i].Type)
			{
			case ESkillEffectType::Cast:
				break;
			case ESkillEffectType::Play:
				PlayEffect = mSkillDataArray[mUseSkillIndex].SkillEffectArray[i].Effect;
				break;
			case ESkillEffectType::Trail:
				TrailEffect = mSkillDataArray[mUseSkillIndex].SkillEffectArray[i].Effect;
				break;
			}

		}

		AParticleCascade* Particle = GetWorld()->SpawnActor<AParticleCascade>(CastLoc, GetActorForwardVector().Rotation(), SpawnParam);


		Skill->SetParticle(TrailEffect);
		Particle->SetParticle(PlayEffect);
		Skill->SetCollisionProfile(TEXT("MonsterAttack"));
		Skill->SetDistance(mSkillDataArray[mUseSkillIndex].Distance);


		Particle->SetLifeSpan(1.f);
		Skill->SetLifeSpan(5.f);
	}
	
}

void ASevarog::Skill3()
{

	if (mUseSkillIndex == -1)
		return;

	AAIController* MonsterController = Cast<AAIController>(GetController());

	APlayerCharacter* Target = Cast<APlayerCharacter>(MonsterController->GetBlackboardComponent()->GetValueAsObject(TEXT("Target")));

	if (IsValid(Target))
	{
		FActorSpawnParameters	SpawnParam;
		SpawnParam.SpawnCollisionHandlingOverride =	ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		AParticleCascade* Skill = GetWorld()->SpawnActor<AParticleCascade>(Target->GetActorLocation(), GetActorRotation(), SpawnParam);

		UParticleSystem* PlayEffect = nullptr;
		UParticleSystem* TrailEffect = nullptr;

		int32	EffectCount = mSkillDataArray[mUseSkillIndex].SkillEffectArray.Num();

		for (int32 i = 0; i < EffectCount; ++i)
		{
			switch (mSkillDataArray[mUseSkillIndex].SkillEffectArray[i].Type)
			{
			case ESkillEffectType::Cast:
				break;
			case ESkillEffectType::Play:
				PlayEffect = mSkillDataArray[mUseSkillIndex].SkillEffectArray[i].Effect;
				break;
			case ESkillEffectType::Trail:
				break;
			}
		}

		Skill->SetParticle(PlayEffect);
		Skill->SetLifeSpan(2.f);

		float	DmgRatio = 0.f;

		int32	OptionCount = mSkillDataArray[mUseSkillIndex].SkillOptionArray.Num();

		for (int32 i = 0; i < OptionCount; ++i)
		{
			if (mSkillDataArray[mUseSkillIndex].SkillOptionArray[i].Type ==
				ESkillOptionType::Damage)
				DmgRatio += mSkillDataArray[mUseSkillIndex].SkillOptionArray[i].Option;
		}

		DmgRatio >= 0.f ? 1.f : DmgRatio;

		Target->DamageTake((float)mInfo.Info.AttackPoint * DmgRatio);
	}
	
}

void ASevarog::Spawn()
{
	FActorSpawnParameters SpawnParam;
	SpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	FVector SpawnLocation = FVector(GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z + GetCapsuleComponent()->GetScaledCapsuleHalfHeight());

	AParticleCascade* Particle = GetWorld()->SpawnActor<AParticleCascade>(SpawnLocation, GetActorRotation(), SpawnParam);


	Particle->SetParticle(mInfo.Effect.SpawnEffect);
	Particle->SetLifeSpan(5.f);
	Particle->SetActorScale3D(FVector(3.f, 3.f, 3.f));
}

void ASevarog::DeathEvent()
{
}

