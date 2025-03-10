// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster.h"
#include "../PrisonerGameInstance.h"
#include "MonsterSpawnPoint.h"
#include "MonsterAnimInstance.h"
#include "MonsterAIController.h"
#include "../UMG/MonsterHPBase.h"
#include "../Player/PlayerCharacter.h"

// Sets default values
AMonster::AMonster() :
	m_Ratio(1.f)
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	mWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetCom"));
	mWidgetComponent->SetupAttachment(GetMesh());

	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Monster"));
	GetCapsuleComponent()->SetGenerateOverlapEvents(true);
	GetCapsuleComponent()->SetNotifyRigidBodyCollision(true);
	GetCapsuleComponent()->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	AIControllerClass = AMonsterAIController::StaticClass();

	FString EnumString = UEnum::GetValueAsString(mMonsterType);
	mMonsterTableRowName = FName(*EnumString);

	mDissolveEnable = false;
	mDissolveSpawnEnable = false;
	mDissolveMax = 1.5;
	mDissolveMin = -1.f;
	mDissolveTimeMax = 1.12f;
	mDissolveTime = 0.f;

	mDissolve = 0.f;
	mDissolveRange = mDissolveMax - mDissolveMin;



	//스킬 관련 변수 초기화
	mSkillEnable = false;

	mUseSkillIndex = -1;

}

void AMonster::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
}

// Called when the game starts or when spawned
void AMonster::BeginPlay()
{
	Super::BeginPlay();

	
	UPrisonerGameInstance* GameInst = GetWorld()->GetGameInstance<UPrisonerGameInstance>();

	const FMonsterInfo* Info = GameInst->FindMonsterInfoTable(mMonsterTableRowName);
	
	if (Info)
	{
		mInfo.AttackDistance = Info->AttackDistance;
		mInfo.TraceDistance = Info->TraceDistance;
		mInfo.Type = Info->Type;
		mInfo.Info = Info->Info;
		mInfo.Effect = Info->Effect;

		GetCharacterMovement()->MaxWalkSpeed = mInfo.Info.MoveSpeed;
		GetCharacterMovement()->MaxWalkSpeedCrouched = mInfo.Info.MoveSpeed * 0.5f;

		GetMesh()->SetSkeletalMesh(Info->Info.Mesh);
		GetMesh()->SetAnimInstanceClass(Info->Info.AnimClass);

		int32 IndexCount = mDissolveMtrlIndexArray.Num();

		for (int32 i = 0; i < IndexCount; i++)
		{
			UMaterialInstanceDynamic* Mtrl = GetMesh()->CreateDynamicMaterialInstance(mDissolveMtrlIndexArray[i].Index,
				mDissolveMtrlIndexArray[i].Mtrl);

			mDissolveMtrlArray.Add(Mtrl);
		}

	}

	mAnimInst = Cast<UMonsterAnimInstance>(GetMesh()->GetAnimInstance());

	if (IsValid(mWidgetComponent))
	{
		UMonsterHPBase* HPWidget = Cast<UMonsterHPBase>(mWidgetComponent->GetWidget());
		if (IsValid(HPWidget))
		{
			HPWidget->SetInitHP(1.f);
		}
	}

	int32 SkillCount = mSkillNameArray.Num();

	for (int32 i = 0; i < SkillCount; ++i)
	{
		const FSkillData* Data = GameInst->FindMonsterSkillTable(mSkillNameArray[i]);

		FMonsterSkillInfo	SkillInfo;

		SkillInfo.Type = Data->Type;
		SkillInfo.System = Data->System;
		SkillInfo.SkillName = Data->SkillName;
		SkillInfo.Description = Data->Description;
		SkillInfo.SkillOptionArray = Data->SkillOptionArray;
		SkillInfo.SkillEffectArray = Data->SkillEffectArray;
		SkillInfo.SkillUseDataArray = Data->SkillUseDataArray;
		SkillInfo.CooldownTime = Data->CooldownTime;

		SkillInfo.UseSkill = false;
		SkillInfo.UseMulti = false;

		/*int32 UseCount = SkillInfo.SkillUseDataArray.Num();

		for (int32 j = 0; j < UseCount; ++j)
		{
			if (SkillInfo.SkillUseDataArray[j].Type == ESkillUseType::Duration)
			{
				SkillInfo.UseMulti = true;
				break;
			}
		}*/

		SkillInfo.Duration = 0.f;
		SkillInfo.CooldownTimeBase = 0.f;
		SkillInfo.Distance = Data->Distance;
		SkillInfo.AnimType = Data->AnimType;

		mSkillDataArray.Add(SkillInfo);
	}
}

void AMonster::OnDissolve()
{
	if (mDissolveMtrlArray.IsEmpty())
		return;

	mDissolveEnable = true;

	int32 Count = mDissolveMtrlArray.Num();
	for (int32 i = 0; i < Count; i++)
	{
		mDissolveMtrlArray[i]->SetScalarParameterValue(TEXT("DissolveEnable"), 1.f);
	}
}

void AMonster::OnSpawnDissolve()
{
	if (mDissolveMtrlArray.IsEmpty())
		return;

	mDissolveSpawnEnable = true;

	int32 Count = mDissolveMtrlArray.Num();
	for (int32 i = 0; i < Count; i++)
	{
		mDissolveMtrlArray[i]->SetScalarParameterValue(TEXT("DissolveEnable"), 1.f);
	}
}

void AMonster::OnRagdoll()
{
	/*GetMesh()->SetCollisionProfileName("Ragdoll");
	GetMesh()->SetAllBodiesSimulatePhysics(true);
	GetMesh()->SetSimulatePhysics(true);

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->SetPhysicsBlendWeight(1.0f);*/
}

// Called every frame
void AMonster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (mDissolveEnable)
	{
		mDissolveTime += DeltaTime;

		float Ratio = mDissolveTime / mDissolveTimeMax;

		mDissolve = mDissolveMax - Ratio * mDissolveRange;

		int32 Count = mDissolveMtrlArray.Num();
		for (int32 i = 0; i < Count; i++)
		{
			mDissolveMtrlArray[i]->SetScalarParameterValue(TEXT("Dissolve"), mDissolve);
		}

		if (mDissolveTime >= mDissolveTimeMax)
		{
			if (IsValid(mSpawnPoint))
			{
				mSpawnPoint->RemoveMonster(this);
				mAnimInst->ChangeAnim(EMonsterAnimType::Idle);
			}
			else
				Destroy();
			mDissolve = 0.f;
			mDissolveTime = 0.f;
			mDissolveEnable = false;

			FString CurrentLevelName = UGameplayStatics::GetCurrentLevelName(this);
			if (CurrentLevelName == FString(TEXT("Boss")))
			{
				APrisonerGameModeBase* GameMode = GetWorld()->GetAuthGameMode<APrisonerGameModeBase>();

				if (IsValid(GameMode))
				{
					GameMode->VisibleGameClearHUD();
				}
			}
		}

	}

	if (mDissolveSpawnEnable)
	{
		mDissolveTime += DeltaTime;

		float Ratio = mDissolveTime / mDissolveSpawnTimeMax;

		mDissolve = mDissolveMin + Ratio * mDissolveRange;
		int32 Count = mDissolveMtrlArray.Num();
		for (int32 i = 0; i < Count; i++)
		{
			mDissolveMtrlArray[i]->SetScalarParameterValue(TEXT("Dissolve"), mDissolve);
		}
		if (mDissolveTime >= mDissolveSpawnTimeMax)
		{
			mDissolve = 0.f;
			mDissolveTime = 0.f;
			mDissolveSpawnEnable = false;

			int32 Count2 = mDissolveMtrlArray.Num();
			for (int32 i = 0; i < Count2; i++)
			{
				mDissolveMtrlArray[i]->SetScalarParameterValue(TEXT("DissolveEnable"), -1.f);
			}
		}
	}

	if (!mSkillEnable)
		UseSkill(DeltaTime);
}

// Called to bind functionality to input
void AMonster::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

float AMonster::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	/*int32 Damage = (int32)Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	Damage = Damage - mInfo.Info.ArmorPoint;

	Damage = Damage < 1 ? 1 : Damage;

	mInfo.Info.HP -= Damage;

	if (mInfo.Info.HP <= 0)
	{
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		mAnimInst->ChangeAnim(EMonsterAnimType::Death);

		AAIController* AI = Cast<AAIController>(GetController());

		if (IsValid(AI))
		{
			AI->BrainComponent->StopLogic(TEXT("Death"));
		}
	}
	else
	{
		mAnimInst->Hit();
	}*/
	return 0.f;
}

float AMonster::DamageTake(float DamageAmount)
{
	int32 Damage = DamageAmount;

	Damage = Damage - mInfo.Info.ArmorPoint;

	Damage = Damage < 1 ? 1 : Damage;

	mInfo.Info.HP -= Damage;

	if (IsValid(mWidgetComponent))
	{
		UMonsterHPBase* MonsterHp = Cast<UMonsterHPBase>(mWidgetComponent->GetWidget());

		if (IsValid(MonsterHp))
		{
			float HPRatio = (float)mInfo.Info.HP / (float)mInfo.Info.HPMax;
			MonsterHp->SetHP(HPRatio);
		}
	}
	


	if (mInfo.Info.HP <= 0)
	{
		Death();

		APlayerCharacter* Player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));

		if (IsValid(Player))
		{
			//Player->AddExp(mInfo.Info.Exp);
			Player->AddExp(20);
		}


		APrisonerGameModeBase* GameMode = GetWorld()->GetAuthGameMode<APrisonerGameModeBase>();

		if (IsValid(GameMode))
		{
			GameMode->AddMonsterDeathCount();
		}
	}
	else
	{
		mAnimInst->Hit();
	}

	return 0.0f;
}



void AMonster::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	AMonsterAIController* AI = Cast<AMonsterAIController>(NewController);

	if (IsValid(AI))
	{
		AI->SetBehaviorTree(TEXT("/Script/AIModule.BehaviorTree'/Game/Monster/AI/BTMonster.BTMonster'"));
		AI->SetBlackboard(TEXT("/Script/AIModule.BlackboardData'/Game/Monster/AI/BBMonster.BBMonster'"));
	}
}

void AMonster::UnPossessed()
{
	Super::UnPossessed();
}

void AMonster::Attack()
{
}

void AMonster::Spawn()
{
}

void AMonster::DeathEvent()
{
}

void AMonster::RestartMonster()
{
	AAIController* AI = Cast<AAIController>(GetController());
	if (IsValid(AI))
	{
		AI->BrainComponent->RestartLogic();
	}
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	mAnimInst->ChangeAnim(EMonsterAnimType::Spawn);
	SetAttackEnd(false);
	//부활 시 현재HP를 초기화
	UPrisonerGameInstance* GameInst = GetWorld()->GetGameInstance<UPrisonerGameInstance>();
	const FMonsterInfo* Info = GameInst->FindMonsterInfoTable(mMonsterTableRowName);

	if (Info)
	{
		mInfo.Info = Info->Info;
	}

	//HP_Bar도 초기화
	UMonsterHPBase* MonsterHp = Cast<UMonsterHPBase>(mWidgetComponent->GetWidget());
	if (IsValid(MonsterHp))
	{
		MonsterHp->SetHP(1);
	}

	MonsterHp->SetVisibility(ESlateVisibility::Visible);

	
}

void AMonster::Death()
{
	if (IsValid(mWidgetComponent))
	{
		UMonsterHPBase* MonsterHp = Cast<UMonsterHPBase>(mWidgetComponent->GetWidget());
		if (IsValid(MonsterHp))
			MonsterHp->SetVisibility(ESlateVisibility::Collapsed);
	}
	

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	mAnimInst->ChangeAnim(EMonsterAnimType::Death);
	DeathEvent();
	SetAttackEnd(false);
	AAIController* AI = Cast<AAIController>(GetController());
	if (IsValid(AI))
	{
		AI->BrainComponent->StopLogic(TEXT("Death"));
	}
}

void AMonster::UseSkill(float DeltaTime)
{
	//전투 상태인지 판단
	AMonsterAIController* AIController = Cast<AMonsterAIController>(GetController());
	if (!IsValid(AIController))
		return;

	APlayerCharacter* Target = Cast<APlayerCharacter>(AIController->GetBlackboardComponent()->GetValueAsObject(TEXT("Target")));
	if (!Target)
		return;

	//타겟과의 거리 체크.
	float HalfHeight = 0.f;
	if (Cast<ACharacter>(Target))
		HalfHeight = Cast<ACharacter>(Target)->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();

	FVector TargetLoc = Target->GetActorLocation();
	TargetLoc.Z = HalfHeight;

	FVector Loc = GetActorLocation();
	Loc.Z -= GetCapsuleComponent()->GetScaledCapsuleHalfHeight();

	float Direction = (float)FVector::Distance(TargetLoc, Loc);

	Direction -= GetCapsuleComponent()->GetScaledCapsuleRadius();
	Direction -= Cast<ACharacter>(Target)->GetCapsuleComponent()->GetScaledCapsuleRadius();


	//PrintViewport(5.f, FColor::Green, FString::Printf(TEXT("DistanceCheck!")));
	
	int32 SkillCount = mSkillDataArray.Num();
	for (int32 i = 0; i < SkillCount; i++)
	{
		if (mSkillDataArray[i].UseSkill)
		{
			if (!mSkillDataArray[i].UseMulti)
			{
				continue;
			}
		}

		//PrintViewport(5.f, FColor::Green, FString::Printf(TEXT("UseSkill False!!")));


		int32 UseDataCount = mSkillDataArray[i].SkillUseDataArray.Num();

		bool Use = true;

		for (int32 j = 0; j < UseDataCount; j++)
		{
			switch (mSkillDataArray[i].SkillUseDataArray[j].Type)
			{
			case ESkillUseType::HPPercent:
				// 체력계산.
				if ((float)mInfo.Info.HP / mInfo.Info.HPMax >
					mSkillDataArray[i].SkillUseDataArray[j].Data)
				{
					Use = false;
				}
				break;
			case ESkillUseType::Duration:
				mSkillDataArray[i].Duration += DeltaTime;

				if (mSkillDataArray[i].Duration <
					mSkillDataArray[i].SkillUseDataArray[j].Data)
				{
					Use = false;
				}
				break;
			case ESkillUseType::Ratio:
				float Percent;
				Percent = FMath::FRand();

				if (Percent > mSkillDataArray[i].SkillUseDataArray[j].Data)
					Use = false;
				break;
			}

			if (!Use)
				break;
		}

		mSkillDataArray[i].CooldownTimeBase += DeltaTime;

		if (mSkillDataArray[i].CooldownTime >= mSkillDataArray[i].CooldownTimeBase)
		{
			Use = false;
		}

		//PrintViewport(5.f, FColor::Green, FString::Printf(TEXT("mSKillEnable : %b"), mSkillEnable));

		if (!mSkillEnable)
		{
			// 지금 시점에 설정된 스킬 거리보다 멀다면 false;
			if (Direction > mSkillDataArray[i].Distance)
				Use = false;

			// 모두 통과했으므로 스킬을 사용해야 한다.
			if (Use)
			{
				mSkillDataArray[i].UseSkill = true;
				mSkillEnable = true;
				mUseSkillIndex = i;

				AIController->GetBlackboardComponent()->SetValueAsBool(TEXT("SkillEnable"), true);

				//PrintViewport(5.f, FColor::Green, FString::Printf(TEXT("SkillEnabled!")));

			}
		}
	}

}

void AMonster::ClearSkill()
{
	mUseSkillIndex = -1;
	mSkillEnable = false;

	int32 SkillCount = mSkillDataArray.Num();

	for (int32 i = 0; i < SkillCount; i++)
	{
		mSkillDataArray[i].UseSkill = false;
		mSkillDataArray[i].Duration = 0.f;
		mSkillDataArray[i].CooldownTimeBase = 0.f;
	}

	AMonsterAIController* AIConstroller = Cast<AMonsterAIController>(GetController());

	if (IsValid(AIConstroller))
		AIConstroller->GetBlackboardComponent()->SetValueAsBool(TEXT("SkillEnable"), false);
}

void AMonster::ClearCurrentSkill()
{
	if (mUseSkillIndex == -1)
		return;

	mSkillDataArray[mUseSkillIndex].UseSkill = false;
	mSkillDataArray[mUseSkillIndex].Duration = 0.f;
	mSkillDataArray[mUseSkillIndex].CooldownTimeBase = 0.f;


	mUseSkillIndex = -1;
	mSkillEnable = false;

	AMonsterAIController* AIConstroller = Cast<AMonsterAIController>(GetController());

	if (IsValid(AIConstroller))
		AIConstroller->GetBlackboardComponent()->SetValueAsBool(TEXT("SkillEnable"), false);
}

void AMonster::Skill1()
{
}

void AMonster::Skill2()
{
}

void AMonster::Skill3()
{
}

