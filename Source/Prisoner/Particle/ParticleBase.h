// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "GameFramework/Actor.h"
#include "ParticleBase.generated.h"

UCLASS()
class PRISONER_API AParticleBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AParticleBase();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
		UAudioComponent* mAudio;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	void SetSound(const FString& Path, bool Play = true);
	void SetSound(USoundBase* Sound, bool Play = true);
	virtual void SetParticle(UParticleSystem* Particle);
	virtual void SetParticle(UNiagaraSystem* Particle);
	virtual void SetParticle(const FString& Path);

};
