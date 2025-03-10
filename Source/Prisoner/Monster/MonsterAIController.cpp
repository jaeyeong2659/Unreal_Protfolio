// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterAIController.h"

AMonsterAIController::AMonsterAIController()
{
}

void AMonsterAIController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);

	if (IsValid(mAITree) && IsValid(mAIBlackboard))
	{
		UBlackboardComponent* BlackboardRef = Blackboard;
		if (UseBlackboard(mAIBlackboard, BlackboardRef))
		{
			RunBehaviorTree(mAITree);
		}
	}
}

void AMonsterAIController::OnUnPossess()
{
	Super::OnUnPossess();
}

void AMonsterAIController::SetBehaviorTree(const FString& Path)
{
	mAITree = LoadObject<UBehaviorTree>(nullptr, *Path);
}

void AMonsterAIController::SetBlackboard(const FString& Path)
{
	mAIBlackboard = LoadObject<UBlackboardData>(nullptr, *Path);
}
