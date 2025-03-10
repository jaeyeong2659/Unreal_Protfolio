// Fill out your copyright notice in the Description page of Project Settings.


#include "TriggerShape.h"

// Sets default values
ATriggerShape::ATriggerShape()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	mRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	SetRootComponent(mRoot);

	mRoot->bVisualizeComponent = true;

	mShape = ETriggerShape::Box;

}

void ATriggerShape::ChangeShape(ETriggerShape _Shape)
{
	//입력 받은게 동일하다면 ?
	if (mShape == _Shape)
		return;

	//기존에 있던것들 삭제 및 초기화
	if (mBox)
	{
		mBox->DetachFromParent();
		mBox->DestroyComponent();
		mBox = nullptr;
	}
	else if (mSphere)
	{
		mSphere->DetachFromParent();
		mSphere->DestroyComponent();
		mSphere = nullptr;
	}
	else if (mCapsule)
	{
		mCapsule->DetachFromParent();
		mCapsule->DestroyComponent();
		mCapsule = nullptr;
	}

	switch (_Shape)
	{
	case ETriggerShape::Box:
		mBox = NewObject<UBoxComponent>(this, UBoxComponent::StaticClass(), TEXT("Box"));
		mBox->RegisterComponent();
		mBox->AttachToComponent(mRoot, FAttachmentTransformRules::KeepRelativeTransform);

		mBox->OnComponentBeginOverlap.AddDynamic(this, &ATriggerShape::BeginOverlap);
		mBox->OnComponentEndOverlap.AddDynamic(this, &ATriggerShape::EndOverlap);
		break;
	case ETriggerShape::Sphere:
		mSphere = NewObject<USphereComponent>(this, USphereComponent::StaticClass(), TEXT("Sphere"));
		mSphere->RegisterComponent();
		mSphere->AttachToComponent(mRoot, FAttachmentTransformRules::KeepRelativeTransform);

		mSphere->OnComponentBeginOverlap.AddDynamic(this, &ATriggerShape::BeginOverlap);
		mSphere->OnComponentEndOverlap.AddDynamic(this, &ATriggerShape::EndOverlap);
		break;
	case ETriggerShape::Capsule:
		mCapsule = NewObject<UCapsuleComponent>(this, UCapsuleComponent::StaticClass(), TEXT("Capsule"));
		mCapsule->RegisterComponent();
		mCapsule->AttachToComponent(mRoot, FAttachmentTransformRules::KeepRelativeTransform);

		mCapsule->OnComponentBeginOverlap.AddDynamic(this, &ATriggerShape::BeginOverlap);
		mCapsule->OnComponentEndOverlap.AddDynamic(this, &ATriggerShape::EndOverlap);
		break;
	}

	mShape = _Shape;
}

void ATriggerShape::ChangeCollisionProfile(const FString& Name)
{
	switch (mShape)
	{
	case ETriggerShape::Box:
		mBox->SetCollisionProfileName(*Name);
		break;
	case ETriggerShape::Sphere:
		mSphere->SetCollisionProfileName(*Name);
		break;
	case ETriggerShape::Capsule:
		mCapsule->SetCollisionProfileName(*Name);
		break;
	}
}

void ATriggerShape::EnableCollision(bool _Enable)
{
	switch (mShape)
	{
	case ETriggerShape::Box:
		if (!_Enable)
			mBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		else
			mBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		break;
	case ETriggerShape::Sphere:
		if (!_Enable)
			mSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		else
			mSphere->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		break;
	case ETriggerShape::Capsule:
		if (!_Enable)
			mCapsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		else
			mCapsule->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		break;
	}
}

void ATriggerShape::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	if (mBox && mShape == ETriggerShape::Box)
		return;

	else if (mSphere && mShape == ETriggerShape::Sphere)
		return;

	else if (mCapsule && mShape == ETriggerShape::Capsule)
		return;

	if (mBox)
	{
		mBox->DetachFromParent();
		mBox->DestroyComponent();
		mBox = nullptr;
	}

	else if (mSphere)
	{
		mSphere->DetachFromParent();
		mSphere->DestroyComponent();
		mSphere = nullptr;
	}

	else if (mCapsule)
	{
		mCapsule->DetachFromParent();
		mCapsule->DestroyComponent();
		mCapsule = nullptr;
	}

	switch (mShape)
	{
	case ETriggerShape::Box:
		mBox = NewObject<UBoxComponent>(this, UBoxComponent::StaticClass(), TEXT("Box"));
		mBox->RegisterComponent();
		mBox->AttachToComponent(mRoot, FAttachmentTransformRules::KeepRelativeTransform);

		mBox->OnComponentBeginOverlap.AddDynamic(this, &ATriggerShape::BeginOverlap);
		mBox->OnComponentEndOverlap.AddDynamic(this, &ATriggerShape::EndOverlap);
		break;
	case ETriggerShape::Sphere:
		mSphere = NewObject<USphereComponent>(this, USphereComponent::StaticClass(), TEXT("Sphere"));
		mSphere->RegisterComponent();
		mSphere->AttachToComponent(mRoot, FAttachmentTransformRules::KeepRelativeTransform);

		mSphere->OnComponentBeginOverlap.AddDynamic(this, &ATriggerShape::BeginOverlap);
		mSphere->OnComponentEndOverlap.AddDynamic(this, &ATriggerShape::EndOverlap);
		break;
	case ETriggerShape::Capsule:
		mCapsule = NewObject<UCapsuleComponent>(this, UCapsuleComponent::StaticClass(), TEXT("Capsule"));
		mCapsule->RegisterComponent();
		mCapsule->AttachToComponent(mRoot, FAttachmentTransformRules::KeepRelativeTransform);

		mCapsule->OnComponentBeginOverlap.AddDynamic(this, &ATriggerShape::BeginOverlap);
		mCapsule->OnComponentEndOverlap.AddDynamic(this, &ATriggerShape::EndOverlap);
		break;
	}

	ChangeCollisionProfile(TEXT("PlayerTrigger"));
}

// Called when the game starts or when spawned
void ATriggerShape::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATriggerShape::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATriggerShape::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	TriggerBegin(SweepResult);
}

void ATriggerShape::EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	TriggerEnd();
}

void ATriggerShape::TriggerBegin(const FHitResult& SweepResult)
{
}

void ATriggerShape::TriggerEnd()
{
}

