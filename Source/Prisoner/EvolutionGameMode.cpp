// Fill out your copyright notice in the Description page of Project Settings.


#include "EvolutionGameMode.h"
#include "Evolution/EvolutionSelectPawn.h"
#include "PrisonerGameInstance.h"

AEvolutionGameMode::AEvolutionGameMode()
{
	ConstructorHelpers::FClassFinder<USelectWidget>	finder(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Blueprints/UMG/UI_Select.UI_Select_C'"));

	if (finder.Succeeded())
	{
		m_SelectHUDClass = finder.Class;
	}
}

void AEvolutionGameMode::BeginPlay()
{
	Super::BeginPlay();


	if (IsValid(m_SelectHUDClass))
	{
		m_SelectHUD = Cast<USelectWidget>(CreateWidget(GetWorld(), m_SelectHUDClass));
		if (IsValid(m_SelectHUD))
		{
			m_SelectHUD->AddToViewport();
			m_SelectHUD->SetStartButtonVisibility(false);
		}
	}
}

void AEvolutionGameMode::Tick(float DeltaTIme)
{
	Super::Tick(DeltaTIme);
}

