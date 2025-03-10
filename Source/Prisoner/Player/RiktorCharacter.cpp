// Fill out your copyright notice in the Description page of Project Settings.


#include "RiktorCharacter.h"
#include "../PrisonerGameInstance.h"


ARiktorCharacter::ARiktorCharacter()
{
	GetMesh()->SetRelativeLocation(FVector(0.0, 0.0, -88.0));
	GetMesh()->SetRelativeRotation(FRotator(0.0, -90, 0.0));

	mPlayerInfoTableName = FName("Riktor");
}



void ARiktorCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void ARiktorCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ARiktorCharacter::NomalAttackCheck()
{
	FVector StartLocation = GetActorLocation() + GetActorForwardVector() * 30.f;
	FVector EndLocation = StartLocation + GetActorForwardVector() * mInfo.AttackDistance;

	FCollisionQueryParams param(NAME_None, false, this);

	TArray<FHitResult>	CollisionResult;
	bool CollisionEnable = GetWorld()->SweepMultiByChannel(CollisionResult, StartLocation, EndLocation, FQuat::Identity,
		ECC_GameTraceChannel2, FCollisionShape::MakeSphere(50.f), param);

#if ENABLE_DRAW_DEBUG

	FColor DrawColor = CollisionEnable ? FColor::Red : FColor::Green;

	DrawDebugCapsule(GetWorld(), (StartLocation + EndLocation) / 2, mInfo.AttackDistance / 2, 50.f, FRotationMatrix::MakeFromZ(GetActorForwardVector()).ToQuat(),
		DrawColor, false, 0.5f);

#endif	


	if (CollisionEnable)
	{
		int32 Count = CollisionResult.Num();

		for (int32 i = 0; i < Count; ++i)
		{
			FActorSpawnParameters SpawnParam;
			SpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		}
	}


}

