// Fill out your copyright notice in the Description page of Project Settings.


#include "EvolutionCharacter.h"
#include "../PrisonerGameInstance.h"
#include "../Player/PlayerCharacter.h"
#include "../EvolutionGameMode.h"
#include "EvolutionPlayerController.h"
#include "../UMG/SelectWidget.h"
#include "EvolutionSelectPawn.h"
#include "EvolutionPlayerController.h"
#include "EvolutionAnimInstance.h"
#include "EvolutionCharacterTwo.h"

// Sets default values
AEvolutionCharacter::AEvolutionCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	GetMesh()->SetRelativeLocation(FVector(0.0, 0.0, -88.0));
	GetMesh()->SetRelativeRotation(FRotator(0.0, -90, 0.0));


	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Player"));
	GetCapsuleComponent()->SetGenerateOverlapEvents(true);
	GetCapsuleComponent()->SetNotifyRigidBodyCollision(true);


	FConvertMaterial2	Mtrl;
	Mtrl.Index = 0;

	mDissolveMtrlIndexArray.Add(Mtrl);

	mDissolveEnable = false;
	mDissolveSpawnEnable = false;
	mDissolveMax = 1.5;
	mDissolveMin = -1.f;
	mDissolveTimeMax = 2.f;
	mDissolveTime = 0.f;

	mDissolve = 0.f;
	mDissolveRange = mDissolveMax - mDissolveMin;


}

// Called when the game starts or when spawned
void AEvolutionCharacter::BeginPlay()
{
	Super::BeginPlay();


	// GameInstance�� ����� �÷��̾� ���� ��������
	UPrisonerGameInstance* GameInst = GetWorld()->GetGameInstance<UPrisonerGameInstance>();
	if (!IsValid(GameInst))
	{
		return;
	}
	
	//LevelUp �Լ����� �÷��̾� ������ ���� �� ������ȯ�� �̷�����. �ֽ� ���� �ҷ�����
	mInfo = GameInst->GetPlayerInfo();

	//�÷��̾� ������ ���� � ������ �������� ����
	SetPlayerJob();


	const FSelectJobDes* Info = GameInst->FindSelectJobTable(TechCharacterName);

	if (Info)
	{
		GetMesh()->SetSkeletalMesh(Info->Mesh);
		GetMesh()->SetAnimInstanceClass(Info->AnimClass);
		EvolutionJob = Info->Name;
		EvolutionDec = Info->Desc;
		mPlayerJob = Info->Job;

		int32 IndexCount = mDissolveMtrlIndexArray.Num();

		for (int32 i = 0; i < IndexCount; i++)
		{
			UMaterialInstanceDynamic* Mtrl = GetMesh()->CreateDynamicMaterialInstance(mDissolveMtrlIndexArray[i].Index,
				mDissolveMtrlIndexArray[i].Mtrl);

			mDissolveMtrlArray.Add(Mtrl);
		}
	}

	OnDissolve();
	//���� ��ȭ�� ǥ�õǴ� ĳ���� �ִϸ��̼��� �ٸ� ĳ���� Ŭ���������� �����ϱ� ���ؼ� �ش� Ŭ���� GameMode�� ����
	AEvolutionGameMode* GameMode = GetWorld()->GetAuthGameMode<AEvolutionGameMode>();

	if (IsValid(GameMode))
	{
		GameMode->SetEvolutionCharacter(this);
	}



}

// Called every frame
void AEvolutionCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (mDissolveEnable)
	{
		mDissolveTime += DeltaTime;

		float Ratio = mDissolveTime / mDissolveTimeMax;

		mDissolve = mDissolveMin + Ratio * mDissolveRange;
		int32 Count = mDissolveMtrlArray.Num();
		for (int32 i = 0; i < Count; i++)
		{
			mDissolveMtrlArray[i]->SetScalarParameterValue(TEXT("Dissolve"), mDissolve);
		}
		if (mDissolveTime >= mDissolveTimeMax)
		{
			mDissolve = 0.f;
			mDissolveTime = 0.f;
			mDissolveEnable = false;

			int32 Count2 = mDissolveMtrlArray.Num();
			for (int32 i = 0; i < Count2; i++)
			{
				mDissolveMtrlArray[i]->SetScalarParameterValue(TEXT("DissolveEnable"), -1.f);
			}

			UEvolutionAnimInstance* AnimInst = Cast<UEvolutionAnimInstance>(GetMesh()->GetAnimInstance());

			if (IsValid(AnimInst))
			{
				AnimInst->ChangeAnim(EEvolutionAnimType::Idle);
			}
		}
	}

}

// Called to bind functionality to input
void AEvolutionCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEvolutionCharacter::SetPlayerJob()
{
	UPrisonerGameInstance* GameInst = Cast<UPrisonerGameInstance>(GetWorld()->GetGameInstance());
	ETechType Tech1 = ETechType::None;
	if (IsValid(GameInst))
	{
		Tech1 = GameInst->GetEvolutionTech();
	}

	if (Tech1 == ETechType::Tech_1)
	{
		TechCharacterName = FName(TEXT("Knight"));
	}
	else if (Tech1 == ETechType::Tech_2)
	{
		TechCharacterName = FName(TEXT("Narbash"));
	}
	else if (Tech1 == ETechType::Tech_3)
	{
		TechCharacterName = FName(TEXT("Minion"));
	}
	else if (mInfo.Info.Level == 9)
	{
		TechCharacterName = FName(TEXT("Minion"));
	}
	else 
		TechCharacterName = FName(TEXT("Knight"));
}

void AEvolutionCharacter::SetSelectAnim()
{
	//�ڽ��� ���É�ٸ� Select �ִϸ��̼� ��ȯ�ϰ�, �ٸ�ĳ���ʹ� Idle�� ����
	mAnimInst = Cast<UEvolutionAnimInstance>(GetMesh()->GetAnimInstance());

	mAnimInst->ChangeAnim(EEvolutionAnimType::Select);

	AEvolutionGameMode* GameMode = GetWorld()->GetAuthGameMode<AEvolutionGameMode>();

	if (IsValid(GameMode))
	{
		AEvolutionCharacterTwo* Two = GameMode->GetEvolutionCharacterTwo();

		mAnimInst = Cast<UEvolutionAnimInstance>(Two->GetMesh()->GetAnimInstance());
		mAnimInst->ChangeAnim(EEvolutionAnimType::Idle);
	}
}

void AEvolutionCharacter::OnDissolve()
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

void AEvolutionCharacter::NotifyActorBeginCursorOver()
{
	Super::NotifyActorBeginCursorOver();
}

void AEvolutionCharacter::NotifyActorEndCursorOver()
{
	Super::NotifyActorEndCursorOver();
}

void AEvolutionCharacter::NotifyActorOnClicked(FKey ButtonPressed)
{
	Super::NotifyActorOnClicked(ButtonPressed);

	AEvolutionGameMode* GameMode = GetWorld()->GetAuthGameMode<AEvolutionGameMode>();

	if (IsValid(GameMode))
	{
		GameMode->GetSelectWidget()->SetDesText(EvolutionJob, EvolutionDec);
		GameMode->GetSelectWidget()->SetStartButtonVisibility(true);
		SetSelectAnim();
	}

	UPrisonerGameInstance* GameInst = Cast<UPrisonerGameInstance>(GetWorld()->GetGameInstance());

	if (IsValid(GameInst))
	{
		GameInst->SetSelectJob(mPlayerJob);
	}
}

void AEvolutionCharacter::NotifyActorOnReleased(FKey ButtonReleased)
{
	Super::NotifyActorOnReleased(ButtonReleased);
}

void AEvolutionCharacter::NotifyActorOnInputTouchBegin(const ETouchIndex::Type FingerIndex)
{
	Super::NotifyActorOnInputTouchBegin(FingerIndex);
}

void AEvolutionCharacter::NotifyActorOnInputTouchEnd(const ETouchIndex::Type FingerIndex)
{
	Super::NotifyActorOnInputTouchEnd(FingerIndex);
}

void AEvolutionCharacter::NotifyActorOnInputTouchEnter(const ETouchIndex::Type FingerIndex)
{
	Super::NotifyActorOnInputTouchEnter(FingerIndex);
}

void AEvolutionCharacter::NotifyActorOnInputTouchLeave(const ETouchIndex::Type FingerIndex)
{
	Super::NotifyActorOnInputTouchLeave(FingerIndex);
}

