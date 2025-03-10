// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillActor.h"
#include "../Particle/Decal.h"

// Sets default values
ASkillActor::ASkillActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	mRoot = CreateDefaultSubobject<UBoxComponent>(TEXT("Root"));

	SetRootComponent(mRoot);

	mSkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	mStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	mParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particle"));
	mNiagara = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Niagara"));

	mSound = nullptr;

	mSkeletalMesh->SetupAttachment(mRoot);
	mStaticMesh->SetupAttachment(mRoot);
	mParticle->SetupAttachment(mRoot);
	mNiagara->SetupAttachment(mRoot);

	mRoot->bVisualizeComponent = true;

	mDecalLifeSpan = 0.f;
}

// Called when the game starts or when spawned
void ASkillActor::BeginPlay()
{
	Super::BeginPlay();

	if (IsValid(mSound))
		UGameplayStatics::PlaySoundAtLocation(this, mSound, GetActorLocation());
}

void ASkillActor::EndPlay(
	const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	mOnSkillEnd.Clear();
}

// Called every frame
void ASkillActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASkillActor::SetSkeletalMesh(const FString& Path)
{
	USkeletalMesh* SkeletalMesh = LoadObject<USkeletalMesh>(
		nullptr, *Path);

	if (IsValid(SkeletalMesh))
		mSkeletalMesh->SetSkeletalMesh(SkeletalMesh);
}

void ASkillActor::SetStaticMesh(const FString& Path)
{
	UStaticMesh* StaticMesh = LoadObject<UStaticMesh>(
		nullptr, *Path);

	if (IsValid(StaticMesh))
		mStaticMesh->SetStaticMesh(StaticMesh);
}

void ASkillActor::SetParticle(const FString& Path)
{
	UParticleSystem* Particle = LoadObject<UParticleSystem>(
		nullptr, *Path);

	if (IsValid(Particle))
		mParticle->SetTemplate(Particle);
}

void ASkillActor::SetParticle(UParticleSystem* Particle)
{
	if (IsValid(Particle))
		mParticle->SetTemplate(Particle);
}

void ASkillActor::SetSound(const FString& Path)
{
	mSound = LoadObject<USoundBase>(nullptr, *Path);
}

void ASkillActor::SetNiagara(UNiagaraSystem* Niagara)
{
	if (IsValid(Niagara))
		mNiagara->SetAsset(Niagara);
}

void ASkillActor::SetNiagara(const FString& Path)
{
	UNiagaraSystem* Niagara = LoadObject<UNiagaraSystem>(
		nullptr, *Path);

	if (IsValid(Niagara))
		mNiagara->SetAsset(Niagara);
}

void ASkillActor::SetBoxExtent(const FVector& Extent)
{
	mRoot->SetBoxExtent(Extent);
}

void ASkillActor::SetCollisionProfile(const FName& Profile)
{
	mRoot->SetCollisionProfileName(Profile);
}

void ASkillActor::SetDecalTemplate(ADecal* Decal)
{
	mDecal = Decal;
}

void ASkillActor::CreateDecal(const FHitResult& Hit)
{
	if (!IsValid(mDecal))
		return;

	FActorSpawnParameters	SpawnParam;
	SpawnParam.Template = mDecal;
	SpawnParam.SpawnCollisionHandlingOverride =
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	ADecal* Decal =
		GetWorld()->SpawnActor<ADecal>(
			GetActorLocation(),
			GetActorRotation(),
			SpawnParam);

	//PrintViewport(10.f, FColor::Red, FString::Printf(TEXT("LifeSpan : %.5f"), mDecalLifeSpan));
	Decal->SetLifeSpan(mDecalLifeSpan);

	switch (mDecal->GetSpawnType())
	{
	case EDecalSpawnType::Origin:
		Decal->SetActorLocation(Hit.ImpactPoint);
		Decal->SetDecalRotation(Hit.ImpactNormal.ToOrientationRotator());
		break;
	case EDecalSpawnType::Floor:
	{
		FCollisionQueryParams	param(NAME_None, false, this);

		FHitResult	LineHit;
		bool Collision = GetWorld()->LineTraceSingleByChannel(
			LineHit,
			GetActorLocation(),
			GetActorLocation() + FVector::DownVector * 1000.f,
			ECollisionChannel::ECC_Visibility,
			param);

		if (Collision)
		{
			Decal->SetActorLocation(LineHit.ImpactPoint);
		}
	}
	break;
	}
}