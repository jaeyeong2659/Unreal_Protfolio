// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "PrisonerPlayerController.h"
#include "../Monster/Monster.h"
#include <Blueprint\UserWidget.h>
#include "../UMG/CrosshairWidgetBase.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

UCLASS()
class PRISONER_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Component, meta = (AllowPrivateAccess = true))
	USpringArmComponent* m_SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Component, meta = (AllowPrivateAccess = true))
	UCameraComponent* m_Camera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
		FPlayerInfo mInfo;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
		FName mPlayerInfoTableName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skill, meta = (AllowPrivateAccess = true))
		TArray<FSkillInfo>	mSkillInfoArray;

	float mMoveDir;

	class UPlayerAnimInstance* mAnimInst;


	bool mLeftClick;
	bool mRightClick;
	bool mCheck;

	bool mGrenadeDecalOn;


	//카메라 Y 회전값 제한범위
	const float mMaxPitch = -80.f;
	const float mMinPitch = 50.f;
	float mNewPitch;


	const float mMinCameraLength = 100.f;
	const float mMaxCameraLength = 1000.f;
	float mCameraZoom;

	TSubclassOf<UUserWidget>	m_CrossHairClass;
	UCrosshairWidgetBase* m_CrossHUD;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;

public:
	const FPlayerInfo& GetMPlayerInfo()	const
	{
		return mInfo;
	}

public:
	virtual void UnPossessed() override;
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser);
	virtual float DamageTake(float DamageAmount);

public:
	void MoveFront(float Scale);
	void MoveSide(float Scale);
	virtual void MouseX(float Scale);
	virtual void MouseY(float Scale);
	void CameraZoom(float Scale);



public:
	virtual void MouseLeftClick()
	{
		mLeftClick = true;
	}
	virtual void MouseRightClick()
	{
		mRightClick = true;
		MouseX(0.1f);
	}
	void MouseLeftRelieve()
	{
		mLeftClick = false;
		mCheck = true;
	}
	virtual void MouseRightRelieve()
	{
		mRightClick = false;
	}
	void NomalAttack();
	virtual void NomalAttackCheck();
	virtual void Dash();
	void SetDisableInput(bool _Set);
	virtual void ReadyGrenade();
	void SKill1Key();
	virtual void Grenade();
	virtual void Skill1();
	virtual void UseSkill(int32 SkillNumber);



public:
	void MouseMovement();


	void Evolution();
	void AddExp(int32 _Exp);
	void LevelUp(int32 _AddAttack, int32 _AddArmor, int32 _AddHP);
	void TableNameSet(EPlayerJob _Job);


};
