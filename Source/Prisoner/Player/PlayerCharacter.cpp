// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "../PrisonerGameModeBase.h"
#include "PlayerAnimInstance.h"
#include "../PrisonerGameInstance.h"
#include "../Particle/ParticleCascade.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	m_SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	m_Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));

	m_SpringArm->SetupAttachment(GetMesh());
	m_Camera->SetupAttachment(m_SpringArm);

	m_SpringArm->TargetArmLength = 500.f;
	m_SpringArm->SetRelativeLocation(FVector(0.0, 0.0, 160.0));
	m_SpringArm->SetRelativeRotation(FRotator(-15.0, 90.0, 0.0));

	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Player"));
	GetCapsuleComponent()->SetGenerateOverlapEvents(true);
	GetCapsuleComponent()->SetNotifyRigidBodyCollision(true);

	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	mMoveDir = 0.f;

	//ĳ���� ȸ���Ҷ� ��, �� Ŭ�� �ٸ��� ȸ��.
	mLeftClick = false;
	mRightClick = false;

	mCheck = true;

	mNewPitch = 0.f;

	mCameraZoom = 0.f;

	mGrenadeDecalOn = false;

}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	mAnimInst = Cast<UPlayerAnimInstance>(GetMesh()->GetAnimInstance());


}

void APlayerCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	UPrisonerGameInstance* GameInst = GetWorld()->GetGameInstance<UPrisonerGameInstance>();

	if (IsValid(GameInst))
	{
		if (GameInst->GetPlayerInfo().Info.Level == 0)
		{
			const FPlayerInfo* Info = GameInst->FindPlayerInfoTable(mPlayerInfoTableName);

			if (Info)
			{
				mInfo.AttackDistance = Info->AttackDistance;
				mInfo.Job = Info->Job;
				mInfo.Info = Info->Info;
				mInfo.Effect = Info->Effect;

				GetMesh()->SetSkeletalMesh(Info->Info.Mesh);
				GetMesh()->SetAnimInstanceClass(Info->Info.AnimClass);
			}
		}
		else //�÷��̾� ������ 1�� �ƴ϶�� ������ȭ �� �����ȴٴ� �ǹ��̸�, Mesh�� AnimClass �� ����Ʈ �� �����ؼ� �ϱ⿡ ���� x
		{
			//�ֽ� �÷��̾� ���� ��  Info �� ���� ������ �������� �ٸ��⿡ ���� �ҷ��ͼ� ����
			const FPlayerInfo& Info = GameInst->GetPlayerInfo();

			mInfo.Info = Info.Info;

			EPlayerJob SelectJob = GameInst->GetSelectJob();
			TableNameSet(SelectJob);

			const FPlayerInfo* TempInfo = GameInst->FindPlayerInfoTable(mPlayerInfoTableName);

			if (TempInfo)
			{
				mInfo.AttackDistance = TempInfo->AttackDistance;
				mInfo.Job = TempInfo->Job;
				mInfo.Effect = TempInfo->Effect;

				GetMesh()->SetSkeletalMesh(TempInfo->Info.Mesh);
				GetMesh()->SetAnimInstanceClass(TempInfo->Info.AnimClass);
			}

			FVector PrevLocation = GameInst->GetCharacterLocation();
			SetActorLocation(PrevLocation);
		}

		
	}
}

void APlayerCharacter::UnPossessed()
{
	Super::UnPossessed();
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	MouseMovement();
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis<APlayerCharacter>(TEXT("MoveFront"), this, &APlayerCharacter::MoveFront);
	PlayerInputComponent->BindAxis<APlayerCharacter>(TEXT("MoveSide"), this, &APlayerCharacter::MoveSide);
	PlayerInputComponent->BindAxis<APlayerCharacter>(TEXT("MouseX"), this, &APlayerCharacter::MouseX);
	PlayerInputComponent->BindAxis<APlayerCharacter>(TEXT("MouseY"), this, &APlayerCharacter::MouseY);
	PlayerInputComponent->BindAxis<APlayerCharacter>(TEXT("CameraZoom"), this, &APlayerCharacter::CameraZoom);

	PlayerInputComponent->BindAction<APlayerCharacter>(TEXT("MouseLeftClick"),EInputEvent::IE_Pressed, this, &APlayerCharacter::MouseLeftClick);
	PlayerInputComponent->BindAction<APlayerCharacter>(TEXT("MouseRightClick"),EInputEvent::IE_Pressed, this, &APlayerCharacter::MouseRightClick);
	PlayerInputComponent->BindAction<APlayerCharacter>(TEXT("MouseLeftClick"),EInputEvent::IE_Released, this, &APlayerCharacter::MouseLeftRelieve);
	PlayerInputComponent->BindAction<APlayerCharacter>(TEXT("MouseRightClick"),EInputEvent::IE_Released, this, &APlayerCharacter::MouseRightRelieve);
	PlayerInputComponent->BindAction<APlayerCharacter>(TEXT("NomalAttack"),EInputEvent::IE_Pressed, this, &APlayerCharacter::NomalAttack);
	PlayerInputComponent->BindAction<APlayerCharacter>(TEXT("Dash"),EInputEvent::IE_Pressed, this, &APlayerCharacter::Dash);
	PlayerInputComponent->BindAction<APlayerCharacter>(TEXT("Grenade"),EInputEvent::IE_Pressed, this, &APlayerCharacter::ReadyGrenade);
	PlayerInputComponent->BindAction<APlayerCharacter>(TEXT("Grenade"),EInputEvent::IE_Released, this, &APlayerCharacter::Grenade);
	
}

float APlayerCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	/*int32 Damage = (int32)Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	Damage = Damage - mInfo.Info.ArmorPoint;

	Damage < 1 ? 1 : Damage;

	mInfo.Info.HP -= Damage;
	*/
	return 0.f;
}

float APlayerCharacter::DamageTake(float DamageAmount)
{
	float Damage = int32(DamageAmount);

	Damage -= mInfo.Info.ArmorPoint;

	Damage = Damage < 1 ? 1 : Damage;

	mInfo.Info.HP -= Damage;

	APrisonerGameModeBase* GameMode = GetWorld()->GetAuthGameMode<APrisonerGameModeBase>();

	if (IsValid(GameMode))
	{
		//PrintViewport(3.f, FColor::Red, FString::Printf(TEXT("HP : %f"), mInfo.Info.HP / mInfo.Info.HPMax));
		GameMode->GetMain()->SetHP(mInfo.Info.HP / (float)mInfo.Info.HPMax);
	}


	if (mInfo.Info.HP <= 0)
	{
		mAnimInst->ChangePlayerAnimType(EPlayerAnimType::Death);
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		DisableInput(GetController<APrisonerPlayerController>());
	}
	else
	{

		mAnimInst->Hit();

	}

	return 0.0f;
}

void APlayerCharacter::MoveFront(float Scale)
{
	mMoveDir = Scale;
	 
	if (Scale == 0.f)
		return;


	
	//PrintViewport(1.f, FColor::Red, FString::Printf(TEXT("%f"),mMoveDir));

	
	AddMovementInput(GetActorForwardVector(), Scale);

}

void APlayerCharacter::MoveSide(float Scale)
{
	

	if (mMoveDir == 1.f)
	{
		if (Scale == 0.f)
			mAnimInst->SetMoveDir(0.f);
		else if (Scale == 1.f)
			mAnimInst->SetMoveDir(45.f);
		else if (Scale == -1.f)
			mAnimInst->SetMoveDir(-45.f);
	}
	else if (mMoveDir == 0.f)
	{
		if (Scale == 0.f)
			mAnimInst->SetMoveDir(0.f);
		else if (Scale == 1.f)
			mAnimInst->SetMoveDir(90.f);
		else if (Scale == -1.f)
			mAnimInst->SetMoveDir(-90.f);

	}
	else if (mMoveDir == -1.f)
	{
		if (Scale == 0.f)
			mAnimInst->SetMoveDir(180.f);
		else if (Scale == 1.f)
			mAnimInst->SetMoveDir(135.f);
		else if (Scale == -1.f)
			mAnimInst->SetMoveDir(-135.f);
	}

	if (Scale == 0.f)
		return;


	AddMovementInput(GetActorRightVector(), Scale);
}

void APlayerCharacter::MouseX(float Scale)
{
	if (Scale == 0.f)
		return;

	// ��Ŭ���� ī�޶� ȸ��
	if (mLeftClick)
	{
		//PrintViewport(1.f, FColor::Red, TEXT("LClick!"));
		m_SpringArm->AddRelativeRotation(FRotator(0.f, Scale * 180.f * GetWorld()->GetDeltaSeconds(), 0.f));
	}

	// ��Ŭ���� ī�޶� �ٶ󺸴� �������� ĳ���� ȸ��.
	if (mRightClick && !mLeftClick)
	{
		// ������ ��ư Ŭ�� �� �ѹ��� ����.
		if (mCheck)
		{
			// ĳ���� ȸ��
			GetController()->ClientSetRotation(FRotator(0.f, (m_SpringArm->GetComponentRotation().Yaw), 0.f));
			// ī�޶�� ĳ���Ϳ� ��ӵǾ��ֱ� ������ ĳ���� ȸ�� ���� ����.
			m_SpringArm->SetWorldRotation(FRotator(m_SpringArm->GetComponentRotation().Pitch, GetActorRotation().Yaw, 0.f));

			mCheck = false;
		}
		AddControllerYawInput(Scale * 180.f * GetWorld()->GetDeltaSeconds());
	}

}

void APlayerCharacter::MouseY(float Scale)
{
	if (Scale == 0.f)
		return;



	mNewPitch = m_SpringArm->GetRelativeRotation().Pitch + (Scale * 180.f * GetWorld()->GetDeltaSeconds());

	if(!mAnimInst->GetAimSet())
		mNewPitch = FMath::Clamp(mNewPitch, mMaxPitch, mMinPitch);
	else
		mNewPitch = FMath::Clamp(mNewPitch, mMaxPitch, 20.f);

	if (mLeftClick || mRightClick)
	{
		m_SpringArm->SetRelativeRotation(FRotator(mNewPitch, m_SpringArm->GetRelativeRotation().Yaw, m_SpringArm->GetRelativeRotation().Roll));
		mAnimInst->SetAimPitch(m_SpringArm->GetComponentRotation().Pitch);
	}

}

void APlayerCharacter::CameraZoom(float Scale)
{
	if (Scale == 0.f)
		return;

	if (mAnimInst->GetAimSet())
		return;

	mCameraZoom = m_SpringArm->TargetArmLength;
	mCameraZoom += Scale * -20.f;

	
	mCameraZoom = FMath::Clamp(mCameraZoom, mMinCameraLength, mMaxCameraLength);

	m_SpringArm->TargetArmLength = mCameraZoom;

}


void APlayerCharacter::NomalAttack()
{
	mAnimInst->Attack();
}

void APlayerCharacter::NomalAttackCheck()
{
	
}

void APlayerCharacter::Dash()
{
}

void APlayerCharacter::SetDisableInput(bool _Set)
{
	APrisonerPlayerController* PlayerController = GetController<APrisonerPlayerController>();

	if (_Set)
		DisableInput(PlayerController);
	else
		EnableInput(PlayerController);


}

void APlayerCharacter::ReadyGrenade()
{
}

void APlayerCharacter::SKill1Key()
{
	Skill1();
}

void APlayerCharacter::Grenade()
{
}

void APlayerCharacter::Skill1()
{
}

void APlayerCharacter::UseSkill(int32 SkillNumber)
{
}

void APlayerCharacter::MouseMovement()
{
	// �����ư�� �ȴ����� ���콺 �����Ե� ���µ� ���콺 ��,�� �� Ŭ���� ���� �̵�
	if (mLeftClick && mRightClick && !mAnimInst->GetAimSet())
	{
		// ĳ���� ȸ��
		GetController()->ClientSetRotation(FRotator(0.f, (m_SpringArm->GetComponentRotation().Yaw), 0.f));
		// ī�޶�� ĳ���Ϳ� ��ӵǾ��ֱ� ������ ĳ���� ȸ�� ���� ����.
		m_SpringArm->SetWorldRotation(FRotator(m_SpringArm->GetComponentRotation().Pitch, GetActorRotation().Yaw, 0.f));
		// ���� �̵�
		MoveFront(1.f);
	}
}

void APlayerCharacter::Evolution()
{
	UPrisonerGameInstance* GameInst = GetWorld()->GetGameInstance<UPrisonerGameInstance>();

	if (IsValid(GameInst))
	{
		GameInst->SetPlayerInfo(mInfo);
		GameInst->SetCharacterLocation(GetActorLocation());
		UGameplayStatics::OpenLevel(GetWorld(), FName(TEXT("Evolution")));
	}

}

void APlayerCharacter::AddExp(int32 _Exp)
{
	UPrisonerGameInstance* GameInst = Cast<UPrisonerGameInstance>(GetWorld()->GetGameInstance());

	FName Level = FName(FString::FromInt(mInfo.Info.Level));
	const FLevelUpTable* Info = GameInst->FindLevelUpTable(Level);

	if (Info)
	{
		mInfo.Info.Exp += _Exp;

		if (mInfo.Info.Exp >= Info->RequiredExp)
		{
			LevelUp(Info->AddAttack,Info->AddArmor,Info->AddHP);
		}
	}
}

void APlayerCharacter::LevelUp(int32 _AddAttack, int32 _AddArmor, int32 _AddHP)
{
	mInfo.Info.Exp = 0;
	mInfo.Info.Level++;
	mInfo.Info.HPMax += _AddHP;
	mInfo.Info.HP = mInfo.Info.HPMax;
	mInfo.Info.MP = mInfo.Info.MPMax;
	mInfo.Info.ArmorPoint += _AddArmor;
	mInfo.Info.AttackPoint += _AddAttack;

	//PrintViewport(5.f, FColor::Green, TEXT("LevelUP!!!"));

	

	FActorSpawnParameters SpawnParam;
	SpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	AParticleCascade* Particle = GetWorld()->SpawnActor<AParticleCascade>(GetActorLocation(), GetActorRotation(), SpawnParam);

	Particle->SetParticle(TEXT("/Script/Engine.ParticleSystem'/Game/ParagonMinions/FX/Particles/SharedGameplay/States/LevelUp/P_LevelUp.P_LevelUp'"));
	Particle->SetActorScale3D(FVector(5.f, 5.f, 5.f));
	Particle->SetLifeSpan(0.5f);

	////���� ��ȭ ������ �������� ��. ������ ���Ǿ��� ������ �� �ٷ�
	//if(mInfo.Info.Level == 3 || mInfo.Info.Level == 5 || mInfo.Info.Level == 7)
	//{
	//	FTimerHandle TimerHandle;
	//	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &APlayerCharacter::Evolution, 0.5f, false);
	//}
	

}

void APlayerCharacter::TableNameSet(EPlayerJob _Job)
{

	switch (_Job)
	{
	case EPlayerJob::Knight:
		mPlayerInfoTableName = FName((TEXT("Knight")));
		break;
	case EPlayerJob::Soldier:
		mPlayerInfoTableName = FName((TEXT("Soldier")));
		break;
	case EPlayerJob::Minion:
		mPlayerInfoTableName = FName((TEXT("Minion")));
		break;
	case EPlayerJob::Narbash:
		mPlayerInfoTableName = FName((TEXT("Narbash")));
		break;
	case EPlayerJob::Riktor:
		mPlayerInfoTableName = FName((TEXT("Riktor")));
		break;
	case EPlayerJob::Revenant:
		mPlayerInfoTableName = FName((TEXT("Revenant")));
		break;
	}
}
