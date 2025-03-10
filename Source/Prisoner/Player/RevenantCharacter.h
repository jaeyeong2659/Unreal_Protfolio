// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PlayerCharacter.h"
#include "RevenantCharacter.generated.h"



UCLASS()
class PRISONER_API ARevenantCharacter : public APlayerCharacter
{
	GENERATED_BODY()

public:
	ARevenantCharacter();

protected:
	FVector mZoomCameraLoc;
	FVector mPreCameraLoc;
	FRotator mZoomCameraRot;

	class UPlayerAnimInstance* mAnimInst;

	bool DashEnable;
	bool DashCheck;


	TArray<UMaterialInstanceDynamic*> mDissolveMtrlArray;
	TArray<FConvertMaterial>	mDissolveMtrlIndexArray;
	bool	mDissolveEnable;
	bool	mDissolveSpawnEnable;
	float	mDissolve;
	float	mDissolveRange;
	float	mDissolveTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
		float	mDissolveTimeMax;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
		float	mDissolveSpawnTimeMax;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
		float	mDissolveMin;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
		float	mDissolveMax;

	float DashCameraLangth;
	float CurrentCameraLangth;
	bool DashLangthCheck;


	class ADecal* mGrenadePickDecal;

	float LerpTime;
	FVector CameraLocVec;

	bool DeshCheck2;

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

public:
	virtual void NomalAttackCheck() override;
	virtual void Dash() override;
	void  InitDash();
	void DashParticle();
	void OnDissolve(bool _Set);
	virtual void ReadyGrenade() override;
	virtual void Grenade() override;
	virtual void Skill1() override;
	virtual void UseSkill(int32 SkillNumber)	override;


public:
	virtual void MouseX(float Scale) override;
	virtual void MouseLeftClick() override;
	virtual void MouseRightRelieve() override;
	virtual void MouseRightClick() override;

public:
	UFUNCTION()
		void Skill1End(class ASkillActor* SkillActor,
			const FHitResult& Hit);
	UFUNCTION()
		void GrenadeSkillEnd(class ASkillActor* SkillActor,
			const FHitResult& Hit);
};
