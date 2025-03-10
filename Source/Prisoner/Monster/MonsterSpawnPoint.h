// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "../PrisonerGameModeBase.h"
#include "GameFramework/Actor.h"
#include "MonsterSpawnPoint.generated.h"


UENUM(BlueprintType)
enum class EMonsterStatusType : uint8
{
	Alive,
	Dead
};

UCLASS()
class PRISONER_API AMonsterSpawnPoint : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMonsterSpawnPoint();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
		USceneComponent* mRoot;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
		TSubclassOf<class AMonster>	mSpawnClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
		float	mSpawnTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
		int32 mSpawnCount;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	TMap<class AMonster*, EMonsterStatusType>	mMonsterArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
		int32	mMonsterAlive;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
		int32	mMonsterDead;


	float mTime;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
		int32 TotalMonsterDeadCount;

	bool SpawnLock;
	class UMonsterAnimInstance* mAnimInst;

	

public:
	void RemoveMonster(class AMonster* Monster)
	{
		for(auto& Element : mMonsterArray)
		{
			if (Element.Key == Monster)
			{
				Element.Value = EMonsterStatusType::Dead;
				mMonsterAlive--;
				mMonsterDead++;
				TotalMonsterDeadCount++;

				/*APrisonerGameModeBase* GameMode = GetWorld()->GetAuthGameMode<APrisonerGameModeBase>();

				if (IsValid(GameMode))
				{
					GameMode->AddMonsterDeathCount();
				}*/
			}
		}
	}


	void AllRemoveMonster()
	{
		SetSpawnLock(true);
		for (auto& Element : mMonsterArray)
		{
			if (Element.Value == EMonsterStatusType::Alive)
			{
				AMonster* Monster = Element.Key;
				Element.Value = EMonsterStatusType::Dead;
				mMonsterAlive = 0;
				mMonsterDead = 0;

				if (IsValid(Monster))
				{
					Monster->Death();
				}
			}
		}
	}


	void SetSpawnLock(bool _On)
	{
		SpawnLock = _On;
	}

public:
	virtual void OnConstruction(const FTransform& Transform) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
