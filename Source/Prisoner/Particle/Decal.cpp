// Fill out your copyright notice in the Description page of Project Settings.


#include "Decal.h"

ADecal::ADecal()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	mDecal = CreateDefaultSubobject<UDecalComponent>(TEXT("Decal"));

	mRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	SetRootComponent(mRoot);

	mRoot->bVisualizeComponent = true;

	mDecal->SetupAttachment(mRoot);

	mRoot->SetRelativeRotation(FRotator(-90.f, 0.f, 0.f));

}

void ADecal::SetDecalMaterial(const FString& Path)
{
	UMaterialInterface* Mtrl = LoadObject<UMaterialInterface>(nullptr, *Path);

	if (IsValid(Mtrl))
		mDecal->SetDecalMaterial(Mtrl);
}


// Called when the game starts or when spawned
void ADecal::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ADecal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

