// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "GameFramework/Actor.h"
#include "Decal.generated.h"


UENUM(BlueprintType)
enum class EDecalSpawnType : uint8
{
	Origin,
	Floor
};

UCLASS()
class PRISONER_API ADecal : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADecal();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
		USceneComponent* mRoot;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
		UDecalComponent* mDecal;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
		EDecalSpawnType	mSpawnType;

public:
	EDecalSpawnType GetSpawnType()	const
	{
		return mSpawnType;
	}

	void SetSpawnType(EDecalSpawnType Type)
	{
		mSpawnType = Type;
	}

	void SetDecalRotation(const FRotator& Rot)
	{
		mDecal->SetRelativeRotation(Rot);
	}

	void SetDecalMaterial(const FString& Path);
	void SetDecalVisibility(bool Visible)
	{
		mDecal->SetVisibility(Visible);
	}

	bool GetDecalVisibility() const
	{
		return mDecal->IsVisible();
	}

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
