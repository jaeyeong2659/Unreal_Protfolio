// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "GameFramework/Actor.h"
#include "SkillActor.generated.h"

UCLASS()
class PRISONER_API ASkillActor : public AActor
{
	GENERATED_BODY()

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSkillEnd, ASkillActor*, SkillActor, const FHitResult&, Hit);

public:	
	// Sets default values for this actor's properties
	ASkillActor();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
		UBoxComponent* mRoot;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
		USkeletalMeshComponent* mSkeletalMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
		UStaticMeshComponent* mStaticMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	UParticleSystemComponent* mParticle;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
		UNiagaraComponent* mNiagara;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
		USoundBase* mSound;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
		class ADecal* mDecal;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
		float	mDecalLifeSpan;

public:
	UPROPERTY(BlueprintAssignable)
		FOnSkillEnd	mOnSkillEnd;

public:
	void SetDecalLifeSpan(float LifeSpan)
	{
		mDecalLifeSpan = LifeSpan;
	}

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason);

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	void SetSkeletalMesh(const FString& Path);
	void SetStaticMesh(const FString& Path);
	void SetParticle(const FString& Path);
	void SetParticle(UParticleSystem* Particle);
	void SetSound(const FString& Path);
	void SetNiagara(UNiagaraSystem* Niagara);
	void SetNiagara(const FString& Path);
	void SetBoxExtent(const FVector& Extent);
	void SetCollisionProfile(const FName& Profile);
	void SetDecalTemplate(class ADecal* Decal);
	void CreateDecal(const FHitResult& Hit);

public:
	template <typename T>
	void AddSkillEndDelegate(T* Obj, void (T::* Func)(ASkillActor*))
	{
		mOnSkillEnd.AddDynamic(Obj, Func);
	}
};