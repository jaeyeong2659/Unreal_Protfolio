// Fill out your copyright notice in the Description page of Project Settings.


#include "EvolutionSelectPawn.h"
#include "EvolutionPlayerController.h"

// Sets default values
AEvolutionSelectPawn::AEvolutionSelectPawn()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}


// Called when the game starts or when spawned
void AEvolutionSelectPawn::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AEvolutionSelectPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AEvolutionSelectPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

