// Fill out your copyright notice in the Description page of Project Settings.


#include "RevenantCharacter.h"
#include "../PrisonerGameInstance.h"
#include "../Particle/ParticleCascade.h"
#include "PlayerAnimInstance.h"
#include "../Skill/SkillProjectile.h"
#include "../Particle/Decal.h"
#include "Runtime/Core/Public/Math/UnrealMathUtility.h"


ARevenantCharacter::ARevenantCharacter()
{
	GetMesh()->SetRelativeLocation(FVector(0.0, 0.0, -88.0));
	GetMesh()->SetRelativeRotation(FRotator(0.0, -90, 0.0));

	mPlayerInfoTableName = FName("Revenant");

	mZoomCameraLoc = FVector(300.f,80.f,10.f);
	mZoomCameraRot = FRotator(8.f, 0.f, 0.f);


	ConstructorHelpers::FClassFinder<UUserWidget> finder(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Blueprints/UMG/UI_Crosshair.UI_Crosshair_C'"));

	if (finder.Succeeded())
	{
		m_CrossHairClass = finder.Class;
	}


	DashEnable = false;	
	DashCheck = true;

	mDissolveEnable = false;
	mDissolveSpawnEnable = false;
	mDissolveMax = 1.5;
	mDissolveMin = -1.f;
	mDissolveTimeMax = 1.12f;
	mDissolveTime = 0.f;

	mDissolve = 0.f;
	mDissolveRange = mDissolveMax - mDissolveMin;

	DashCameraLangth = 1000.f;
	DashLangthCheck = false;

	LerpTime = 0.f;

	CameraLocVec = FVector::ZeroVector;

	DeshCheck2 = false;
}



void ARevenantCharacter::BeginPlay()
{
	Super::BeginPlay();



	mPreCameraLoc = m_Camera->GetRelativeLocation();
	CurrentCameraLangth = m_SpringArm->TargetArmLength;

	mAnimInst = Cast<UPlayerAnimInstance>(GetMesh()->GetAnimInstance());

	if (IsValid(m_CrossHairClass))
	{
		m_CrossHUD = Cast<UCrosshairWidgetBase>(CreateWidget(GetWorld(), m_CrossHairClass));
		if (IsValid(m_CrossHUD))
		{
			m_CrossHUD->AddToViewport();
			m_CrossHUD->SetVisibility(ESlateVisibility::Collapsed);
		}
	}



	mDissolveTimeMax = 0.3f;

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


	FSkillInfo SkillGrenade;

	SkillGrenade.SlotNumber = 0;
	SkillGrenade.SkillNumber = 0;
	SkillGrenade.Damage = 500;

	ASkillProjectile* SkillGrenadeProjectile = NewObject<ASkillProjectile>(this, ASkillProjectile::StaticClass());

	SkillGrenade.SkillActor = Cast<ASkillActor>(SkillGrenadeProjectile);

	SkillGrenadeProjectile->SetParticle(TEXT("/Script/Engine.ParticleSystem'/Game/ParagonRevenant/FX/Particles/Revenant/Abilities/Obliterate/FX/P_Revenant_Obliterate_Trail.P_Revenant_Obliterate_Trail'"));
	SkillGrenadeProjectile->SetCollisionProfile(TEXT("PlayerAttack"));

	SkillGrenadeProjectile->mOnSkillEnd.AddDynamic(this, &ARevenantCharacter::GrenadeSkillEnd);

	
	UProjectileMovementComponent* GrenadeProjectile = SkillGrenadeProjectile->GetProjectile();

	GrenadeProjectile->InitialSpeed = 2000.f;
	GrenadeProjectile->ProjectileGravityScale = 0.f;

	mSkillInfoArray.Add(SkillGrenade);


	mGrenadePickDecal = GetWorld()->SpawnActor<ADecal>(FVector::ZeroVector, FRotator::ZeroRotator);
	mGrenadePickDecal->SetDecalMaterial(TEXT("/Script/Engine.MaterialInstanceConstant'/Game/Materials/MTMagicCircle_Inst.MTMagicCircle_Inst'"));
	mGrenadePickDecal->SetSpawnType(EDecalSpawnType::Floor);
	mGrenadePickDecal->SetDecalVisibility(false);

	FString CurrentLevelName = UGameplayStatics::GetCurrentLevelName(this);
	if (CurrentLevelName == FString(TEXT("Boss")))
	{
		SetActorLocation(FVector((3579.827665,0.000000,669.000000)));
	}


}

void ARevenantCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	if (DashEnable)
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
			DashEnable = false;
			mDissolve = 0.f;
			mDissolveTime = 0.f;
		}
	}

	if (mDissolveSpawnEnable)
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
			mDissolveSpawnEnable = false;
			mDissolve = 0.f;
			mDissolveTime = 0.f;
			OnDissolve(false);
		}
	}

	if (DashLangthCheck)
	{
		LerpTime += DeltaTime;

		if (LerpTime <= 3.f)
		{
			DashCameraLangth = FMath::Lerp(DashCameraLangth, 1000.f, LerpTime / 3.f);

			m_SpringArm->TargetArmLength = DashCameraLangth;
		}
	}

	if (DeshCheck2)
	{
		LerpTime += DeltaTime;

		if (LerpTime <= 3.f)
		{

			DashCameraLangth = FMath::Lerp(DashCameraLangth, CurrentCameraLangth, LerpTime / 3.f);

			m_SpringArm->TargetArmLength = DashCameraLangth;
		}
		else
		{
			DeshCheck2 = false;
			LerpTime = 0.f;
		}

		//PrintViewport(1.f, FColor::Red, TEXT("Here?"));
	}


	if (mRightClick)
	{
		//m_Camera->SetRelativeLocation(mZoomCameraLoc);

		CameraLocVec = FMath::Lerp(CameraLocVec, mZoomCameraLoc, 0.5f);

		m_Camera->SetRelativeLocation(CameraLocVec);
	}

	if (!mRightClick)
	{
		//m_Camera->SetRelativeLocation(mPreCameraLoc);
		CameraLocVec = FMath::Lerp(CameraLocVec, mPreCameraLoc, 0.5f);

		m_Camera->SetRelativeLocation(CameraLocVec);
	}

	if (mGrenadeDecalOn)
	{

		FVector CameraLoc = m_Camera->GetComponentLocation();
		FRotator CameraRot = m_Camera->GetComponentRotation();
		FVector EndLoc = CameraLoc + CameraRot.Vector() * mInfo.AttackDistance;

		FCollisionQueryParams param(NAME_None, false, this);
		FHitResult result;
		bool Hit = GetWorld()->LineTraceSingleByChannel(result, CameraLoc, EndLoc, ECollisionChannel::ECC_GameTraceChannel8, param);

		if (Hit)
		{
			if (IsValid(mGrenadePickDecal))
			{
				if(!mGrenadePickDecal->GetDecalVisibility())
					mGrenadePickDecal->SetDecalVisibility(true);

				mGrenadePickDecal->SetActorLocation(result.ImpactPoint);
			}
		}
		else
		{
			if (mGrenadePickDecal->GetDecalVisibility())
			{
				mGrenadePickDecal->SetDecalVisibility(false);
			}
		}
	}
}




void ARevenantCharacter::NomalAttackCheck()
{
	//총구 위치 설정
	FVector MuzzleLoc = GetMesh()->GetSocketLocation(TEXT("Muzzle_02"));
	FVector CameraLoc = m_Camera->GetComponentLocation();
	FRotator CameraRot = m_Camera->GetComponentRotation();

	FVector EndLocation = CameraLoc + CameraRot.Vector() * mInfo.AttackDistance;

	FActorSpawnParameters SpawnParam;
	SpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	AParticleCascade* MuzzleParticle = GetWorld()->SpawnActor<AParticleCascade>(MuzzleLoc, GetActorForwardVector().Rotation(), SpawnParam);

	MuzzleParticle->SetParticle(mInfo.Effect.AttackEffect);

	FCollisionQueryParams param(NAME_None, false, this);

	FHitResult	CollisionResult;
	bool CollisionEnable = GetWorld()->LineTraceSingleByChannel(CollisionResult, CameraLoc, EndLocation, ECollisionChannel::ECC_GameTraceChannel2
		, param);

#if ENABLE_DRAW_DEBUG

	FColor DrawColor = CollisionEnable ? FColor::Red : FColor::Green;

	DrawDebugLine(GetWorld(), CameraLoc, EndLocation, DrawColor,false, 1.f, 2.f);

#endif	


	if (CollisionEnable)
	{

		AParticleCascade* Particle = GetWorld()->SpawnActor<AParticleCascade>(CollisionResult.ImpactPoint,
			CollisionResult.ImpactNormal.Rotation(), SpawnParam);


		Particle->SetParticle(mInfo.Effect.HitPointEffect);

		AMonster* Monster = Cast<AMonster>(CollisionResult.GetActor());

		if (IsValid(Monster))
			Monster->DamageTake(mInfo.Info.AttackPoint);
	}
}

void ARevenantCharacter::Dash()
{
	if (mDissolveEnable)
		return;
	MouseRightRelieve();
	mDissolveEnable = true;
	DashEnable = true;
	FTimerHandle TimerHandle;
	OnDissolve(true);
	DashParticle();
	DashLangthCheck = true;
	CurrentCameraLangth = m_SpringArm->TargetArmLength;
	DashCameraLangth = CurrentCameraLangth;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle,this, &ARevenantCharacter::InitDash, 3.f, false);
	LerpTime = 0.f;

	//이동속도 설정
	GetCharacterMovement()->MaxWalkSpeed = mInfo.Info.MoveSpeed * 1.5f;
	DeshCheck2 = false;
}

void ARevenantCharacter::InitDash()
{
	DashCheck = true;				//데쉬 파티클 3초동안 한번만 실행 안전장치
	mDissolveSpawnEnable = true;	//데쉬 끝나면 플레이어 서서히 보이도록 트리거
	mDissolveEnable = false;	//데쉬 끝나기 전까지 alt 재입력 방지
	GetCharacterMovement()->MaxWalkSpeed = mInfo.Info.MoveSpeed /	 1.5f;
	//m_SpringArm->TargetArmLength = CurrentCameraLangth;
	LerpTime = 0.f;
	DashLangthCheck = false;
	DeshCheck2 = true;
}

void ARevenantCharacter::DashParticle()
{
	if (!DashCheck)
		return;
	
	//데쉬를 할때 한번 크게 터지는 파티클 생성
	FActorSpawnParameters SpawnParam;
	SpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	AParticleCascade* DashParticle = GetWorld()->SpawnActor<AParticleCascade>(GetActorLocation(), GetActorRotation(), SpawnParam);
	DashParticle->SetParticle(TEXT("/Script/Engine.ParticleSystem'/Game/ParagonRevenant/FX/Particles/Revenant/Abilities/Mark/FX/P_Revenant_Mark_Hit.P_Revenant_Mark_Hit'"));
	DashParticle->SetLifeSpan(1.f);

	//플레이어를 따라가는 파티클 생성
	AParticleCascade* Dash = GetWorld()->SpawnActor<AParticleCascade>(GetActorLocation(), GetActorRotation(), SpawnParam);
	Dash->SetParticle(TEXT("/Script/Engine.ParticleSystem'/Game/ParagonRevenant/FX/Particles/Revenant/Abilities/Mark/FX/P_Revenant_Mark_Targeting_Upper.P_Revenant_Mark_Targeting_Upper'"));

	//파티클 충돌 및 유효시간, 크기 설정
	Dash->SetActorEnableCollision(false);
	Dash->SetLifeSpan(3.f);
	//Dash->SetActorScale3D(FVector(10.f, 10.f, 10.f));
	Dash->SetActorRelativeScale3D(FVector(10.f, 10.f, 10.f));

	FAttachmentTransformRules AttachRules(EAttachmentRule::SnapToTarget, true);
	Dash->AttachToComponent(GetRootComponent(), AttachRules);


	//Dash->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform);

	DashCheck = false;
	
}

void ARevenantCharacter::OnDissolve(bool _Set)
{
	float Set = _Set ? 1.f : -1.f;

	if (mDissolveMtrlArray.IsEmpty())
		return;


	int32 Count = mDissolveMtrlArray.Num();
	for (int32 i = 0; i < Count; i++)
	{
		mDissolveMtrlArray[i]->SetScalarParameterValue(TEXT("DissolveEnable"), Set);
	}


}

void ARevenantCharacter::ReadyGrenade()
{
	if (!mRightClick)
		return;

	mGrenadeDecalOn = true;
	mGrenadePickDecal->SetDecalVisibility(true);
}

void ARevenantCharacter::Grenade()
{
	if (!mRightClick ||!mGrenadePickDecal->GetDecalVisibility())
		return;

	mGrenadeDecalOn = false;
	mGrenadePickDecal->SetDecalVisibility(false);


	int32 Count = mSkillInfoArray.Num();

	for (int32 i = 0; i < Count; i++)
	{
		if (mSkillInfoArray[i].SkillNumber == 0)
		{
			FActorSpawnParameters SpawnParam;
			SpawnParam.Template = mSkillInfoArray[i].SkillActor;
			SpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

			
			//FVector StartLoc = m_Camera->GetComponentLocation();
			FVector StartLoc = GetMesh()->GetSocketLocation(TEXT("HoverAttachPoint"));
			FVector TargetLoc = mGrenadePickDecal->GetActorLocation();
			FVector Direction = TargetLoc - StartLoc;

			FRotator TargetRotation = FRotationMatrix::MakeFromX(Direction).Rotator();

			ASkillProjectile* SKill = GetWorld()->SpawnActor<ASkillProjectile>(StartLoc,
				TargetRotation, SpawnParam);

			break;
		}
	}
	/*


	PrintViewport(10.f, FColor::Green, TEXT("Success Grenade!"));*/
	mAnimInst->UseSkill(0); // 수류탄 스킬넘버는 0 이니 찾을 필요 x

}

void ARevenantCharacter::Skill1()
{
	int32 SkillNumber = -1;

	int32 Count = mSkillInfoArray.Num();

	for (int32 i = 0; i < Count; i++)
	{
		if (mSkillInfoArray[i].SkillNumber == 1)
		{
			SkillNumber = mSkillInfoArray[i].SkillNumber;
			break;
		}
	}

	if (SkillNumber == -1)
		return;

	mAnimInst->UseSkill(SkillNumber);

}

void ARevenantCharacter::UseSkill(int32 SkillNumber)
{
	if (SkillNumber == 0)
	{
		Grenade();
		return;
	}
	int32 Count = mSkillInfoArray.Num();

	for (int32 i = 0; i < Count; i++)
	{
		if (mSkillInfoArray[i].SkillNumber == SkillNumber)
		{
			FActorSpawnParameters SpawnParam;
			SpawnParam.Template = mSkillInfoArray[i].SkillActor;
			SpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

			ASkillProjectile* SKill = GetWorld()->SpawnActor<ASkillProjectile>(
				GetActorLocation() + GetActorForwardVector() * 100.f,
				GetActorRotation(),
				SpawnParam);


			//PrintViewport(10.f, FColor::Green, TEXT("Success Skill!"));

			break;
		}
	}
}

void ARevenantCharacter::MouseX(float Scale)
{
	if (Scale == 0.f)
		return;

	// 좌클릭시 카메라만 회전
	if (mLeftClick)
	{
		//PrintViewport(1.f, FColor::Red, TEXT("LClick!"));
		m_SpringArm->AddRelativeRotation(FRotator(0.f, Scale * 180.f * GetWorld()->GetDeltaSeconds(), 0.f));
	}

	// 우클릭시 카메라 바라보는 방향으로 캐릭터 회전.
	if (mRightClick && !mLeftClick)
	{
		// 오른쪽 버튼 클릭 시 한번만 실행.
		if (mCheck)
		{
			// 캐릭터 회전
			GetController()->ClientSetRotation(FRotator(0.f, (m_SpringArm->GetComponentRotation().Yaw), 0.f));
			// 카메라는 캐릭터에 상속되어있기 떄문에 캐릭터 회전 영향 제거.
			m_SpringArm->SetWorldRotation(FRotator(m_SpringArm->GetComponentRotation().Pitch, GetActorRotation().Yaw, 0.f));
			
			

			mCheck = false;
		}
		AddControllerYawInput(Scale * 180.f * GetWorld()->GetDeltaSeconds());
	}

	
}

void ARevenantCharacter::MouseLeftClick()
{
	Super::MouseLeftClick();
	if(mAnimInst->GetAimSet())
	mAnimInst->Attack();
}

void ARevenantCharacter::MouseRightRelieve()
{
	Super::MouseRightRelieve();

	mAnimInst->SetAimSet(false);
	//m_Camera->SetRelativeLocation(mPreCameraLoc);
	m_Camera->SetRelativeRotation(FRotator(0.f, 0.f, 0.f));
	m_SpringArm->TargetArmLength = CurrentCameraLangth;

	if (IsValid(m_CrossHUD))
	{
		m_CrossHUD->SetVisibility(ESlateVisibility::Collapsed);
	}

	mGrenadeDecalOn = false;
	mGrenadePickDecal->SetDecalVisibility(false);
}

void ARevenantCharacter::MouseRightClick()
{
	Super::MouseRightClick();

	if (!DashLangthCheck && !mLeftClick && !mDissolveEnable)
	{
		mAnimInst->SetAimSet(true);
		CameraLocVec = FVector::ZeroVector;
		//m_Camera->SetRelativeLocation(mZoomCameraLoc);
		m_Camera->SetRelativeRotation(mZoomCameraRot);
		CurrentCameraLangth = m_SpringArm->TargetArmLength;
		m_SpringArm->TargetArmLength = 440.f;
		mPreCameraLoc = m_Camera->GetRelativeLocation();

		if (IsValid(m_CrossHUD))
		{
			m_CrossHUD->SetVisibility(ESlateVisibility::Visible);
		}
	}
	
}

void ARevenantCharacter::Skill1End(ASkillActor* SkillActor, const FHitResult& Hit)
{
	SkillActor->Destroy();
}

void ARevenantCharacter::GrenadeSkillEnd(ASkillActor* SkillActor, const FHitResult& Hit)
{
	//Grenade가 바닥에 충돌 하면 폭팔 Particle이 발생함과 동시에 주위로 overlap Sphere를 생성해
	//충돌하는 Monster가 있다면 전부 데미지 처리
	FActorSpawnParameters SpawnParam;
	SpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	AParticleCascade* GrenadeHitParticle = GetWorld()->SpawnActor<AParticleCascade>(Hit.ImpactPoint, Hit.ImpactNormal.Rotation(), SpawnParam);
	GrenadeHitParticle->SetParticle(TEXT("/Script/Engine.ParticleSystem'/Game/ParagonRevenant/FX/Particles/Revenant/Abilities/Ultimate/FX/P_Revenant_Ultimate_Hit.P_Revenant_Ultimate_Hit'"));
	GrenadeHitParticle->SetLifeSpan(1.f);

	FCollisionQueryParams param(NAME_None, false, this);
	TArray<FOverlapResult> ResultArray;

	FVector StartLocation = Hit.ImpactPoint;

	bool CollisionEnable = GetWorld()->OverlapMultiByChannel(ResultArray, StartLocation, FQuat::Identity, ECollisionChannel::ECC_GameTraceChannel2,
		FCollisionShape::MakeSphere(500.f), param);

#if ENABLE_DRAW_DEBUG

	FColor DrawColor = CollisionEnable ? FColor::Red : FColor::Green;

	DrawDebugSphere(GetWorld(), StartLocation, 500.f, 20, DrawColor, false, 0.3f);
#endif	

	if (CollisionEnable)
	{
		int32 Count = ResultArray.Num();

		for (int32 i = 0; i < Count; i++)
		{
			AMonster* Monster = Cast<AMonster>(ResultArray[i].GetActor());
			
			if (IsValid(Monster))
			{
				Monster->DamageTake(mInfo.Info.AttackPoint);


				//데미지를 입히고 몬스터를 사방으로 밀어낸다.
				FVector Direction = Monster->GetActorLocation() - StartLocation;
				Direction.Normalize();

				float PushForce = 800;


				Monster->LaunchCharacter(Direction * PushForce, true, true);
			}
		}
	}
	


	
	SkillActor->Destroy();
}

