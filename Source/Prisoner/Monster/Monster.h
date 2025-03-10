// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include <Components\WidgetComponent.h>
#include "Components/PoseableMeshComponent.h"
#include "GameFramework/Character.h"
#include "Monster.generated.h"

struct FConvertMaterial
{
	int32 Index;
	UMaterialInstanceDynamic* Mtrl;

	FConvertMaterial() :
		Index(-1),
		Mtrl(nullptr)
	{
	}
};


UCLASS()
class PRISONER_API AMonster : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMonster();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
		EMonsterType  mMonsterType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
		FMonsterInfo mInfo;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
		FName mMonsterTableRowName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
		UWidgetComponent* mWidgetComponent;



	float m_Ratio;
	class UMonsterAnimInstance* mAnimInst;
	class AMonsterSpawnPoint* mSpawnPoint;

	bool mAttackEnd;

	
	//Dissolve 관련 함수
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


	//스킬 부분 변수
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
		bool				mSkillEnable;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
		TArray<FName>	mSkillNameArray;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
		TArray<FMonsterSkillInfo>	mSkillDataArray;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	int32	mUseSkillIndex;

	
public:
	int32 GetSkillIndex() const
	{
		return mUseSkillIndex;
	}

	const FMonsterSkillInfo* GetSkillData()
	{
		if (mUseSkillIndex == -1)
			return nullptr;

		return	&mSkillDataArray[mUseSkillIndex];
	}


	const FMonsterInfo& GetMonsterInfo()	const
	{
		return mInfo;
	}
	class UMonsterAnimInstance* GetMonsterAnimInst() const
	{
		return mAnimInst;
	}

	void SetSpawnPoint(class AMonsterSpawnPoint* SpawnPoint)
	{
		mSpawnPoint = SpawnPoint;
	}
	bool GetAttackEnd() const
	{
		return mAttackEnd;
	}

	void SetAttackEnd(bool AttackEnd)
	{
		mAttackEnd = AttackEnd;
	}


public:
	virtual void OnConstruction(const FTransform& Transform) override;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	void OnDissolve();
	void OnSpawnDissolve();
	void OnRagdoll();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser);
	virtual float DamageTake(float DamageAmount);

public:
	virtual void PossessedBy(AController* NewController) override;
	virtual void UnPossessed() override;

public:
	virtual void Attack();
	virtual void Spawn();
	virtual void DeathEvent();

	void RestartMonster();

	void Death();

private:
		void UseSkill(float DeltaTime);

public:
		void ClearSkill();
		void ClearCurrentSkill();
		virtual void Skill1();
		virtual void Skill2();
		virtual void Skill3();
};

