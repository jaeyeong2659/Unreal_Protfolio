// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterSpawnPoint.h"
#include "Monster.h"
#include "MonsterAnimInstance.h"

// Sets default values
AMonsterSpawnPoint::AMonsterSpawnPoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	mRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	SetRootComponent(mRoot);

	mRoot->bVisualizeComponent = true;

	mSpawnTime = 3.f;
	mTime = 3.f;
	mSpawnCount = 1;

	mMonsterAlive = 0;
	mMonsterDead = 0;

	TotalMonsterDeadCount = 0;

	SpawnLock = true;
}

void AMonsterSpawnPoint::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
}

// Called when the game starts or when spawned
void AMonsterSpawnPoint::BeginPlay()
{
	Super::BeginPlay();

	APrisonerGameModeBase* GameMode = GetWorld()->GetAuthGameMode<APrisonerGameModeBase>();
	if (IsValid(GameMode))
	{
		GameMode->AddSpawnPointArray(this);
	}

	
}

// Called every frame
void AMonsterSpawnPoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 설정된 SpawnClass 종류가 선언 되었을 때만 실행.
	if (IsValid(mSpawnClass))
	{
		//MonsterPool에 Dead가 붙은 몬스터가 있고 SpawnLock이 안걸렸을때 아래 코드 실행.
		if (mMonsterDead >= 1 && !SpawnLock)
		{
			mTime -= DeltaTime;

			if (mTime <= 0.f)
			{

				for (auto& Element : mMonsterArray)
				{
					if (Element.Value == EMonsterStatusType::Dead)
					{
						AMonster* Monster = Cast<AMonster>(Element.Key);
						if (IsValid(Monster))
						{
							Element.Value = EMonsterStatusType::Alive;

							Monster->SetActorLocation(GetActorLocation());
							Monster->SetActorRotation(GetActorRotation());

							Monster->RestartMonster();

							Monster->OnSpawnDissolve();
							mMonsterAlive++;
							mMonsterDead--;
							mTime = mSpawnTime;
							break;
						}
					}
				}
			}
		}
		else if (mMonsterArray.Num() < mSpawnCount && !SpawnLock && mMonsterDead == 0)
		{
			// mTime값을 0이하가 될경우에만 실행
			mTime -= DeltaTime;
			if (mTime <= 0.f)
			{
				mTime = mSpawnTime;
				FActorSpawnParameters SpawnParam;
				SpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

				AMonster* SpawnMonster = GetWorld()->SpawnActor<AMonster>(mSpawnClass, GetActorLocation(), GetActorRotation(), SpawnParam);

				SpawnMonster->SetSpawnPoint(this);

				EMonsterStatusType CurrentStatusType = EMonsterStatusType::Alive;
				if (IsValid(SpawnMonster))
				{
					mMonsterArray.Add(SpawnMonster, CurrentStatusType);
					mMonsterAlive++;
				}

				if (mMonsterArray.Num() == mSpawnCount)
					mTime = mSpawnTime;
			}
		}
	}

}

