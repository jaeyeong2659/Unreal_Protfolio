// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "GameFramework/Character.h"
#include "EvolutionCharacter.generated.h"

struct FConvertMaterial2
{
	int32 Index;
	UMaterialInstanceDynamic* Mtrl;

	FConvertMaterial2() :
		Index(-1),
		Mtrl(nullptr)
	{
	}
};


UCLASS()
class PRISONER_API AEvolutionCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEvolutionCharacter();

protected:
	ETechType Tech;
	EPlayerJob mPlayerJob;
	FPlayerInfo mInfo;
	FName TechCharacterName;

	FText EvolutionDec;
	FText EvolutionJob;


	class UEvolutionAnimInstance* mAnimInst;



	TArray<UMaterialInstanceDynamic*> mDissolveMtrlArray;
	TArray<FConvertMaterial2>	mDissolveMtrlIndexArray;
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


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


public:
	void SetPlayerJob();
	void SetSelectAnim();
	void OnDissolve();


public:
	virtual void NotifyActorBeginCursorOver();
	virtual void NotifyActorEndCursorOver();
	virtual void NotifyActorOnClicked(FKey ButtonPressed = EKeys::LeftMouseButton);
	virtual void NotifyActorOnReleased(FKey ButtonReleased = EKeys::LeftMouseButton);
	virtual void NotifyActorOnInputTouchBegin(const ETouchIndex::Type FingerIndex);
	virtual void NotifyActorOnInputTouchEnd(const ETouchIndex::Type FingerIndex);
	virtual void NotifyActorOnInputTouchEnter(const ETouchIndex::Type FingerIndex);
	virtual void NotifyActorOnInputTouchLeave(const ETouchIndex::Type FingerIndex);

};
