// Fill out your copyright notice in the Description page of Project Settings.


#include "ParticleBase.h"

// Sets default values
AParticleBase::AParticleBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	mAudio = CreateDefaultSubobject<UAudioComponent>(TEXT("Audio"));

	SetRootComponent(mAudio);
}

// Called when the game starts or when spawned
void AParticleBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AParticleBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AParticleBase::SetSound(const FString& Path, bool Play)
{
	USoundBase* Sound = LoadObject<USoundBase>(nullptr, *Path);

	if (IsValid(Sound))
	{
		mAudio->SetSound(Sound);

		if (Play)
		{
			mAudio->Play();
		}
	}
}

void AParticleBase::SetSound(USoundBase* Sound, bool Play)
{
	if (IsValid(Sound))
	{
		mAudio->SetSound(Sound);

		if (Play)
			mAudio->Play();
	}
}

void AParticleBase::SetParticle(UParticleSystem* Particle)
{
}

void AParticleBase::SetParticle(UNiagaraSystem* Particle)
{
}

void AParticleBase::SetParticle(const FString& Path)
{
}

