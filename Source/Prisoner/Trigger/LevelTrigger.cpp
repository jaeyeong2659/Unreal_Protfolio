// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelTrigger.h"
#include "../PrisonerGameModeBase.h"
#include "../PrisonerGameInstance.h"
ALevelTrigger::ALevelTrigger()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ALevelTrigger::TriggerBegin(const FHitResult& SweepResult)
{
	APrisonerGameModeBase* GameMode = GetWorld()->GetAuthGameMode<APrisonerGameModeBase>();

	if (IsValid(GameMode))
	{
		GameMode->SetLevelSequenceCheck(true);
		GameMode->SetMainHUDVisibility(false);
	}

	UPrisonerGameInstance* GameInst = Cast<UPrisonerGameInstance>(GetWorld()->GetGameInstance());
	if (IsValid(GameInst))
	{
		GameInst->SetBossMission(true);
	}
	

	UGameplayStatics::OpenLevel(GetWorld(), mLevelName);
}

void ALevelTrigger::TriggerEnd()
{
}
