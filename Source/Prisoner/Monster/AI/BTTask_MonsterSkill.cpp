// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_MonsterSkill.h"
#include "../Monster.h"
#include "../MonsterAIController.h"
#include "../MonsterAnimInstance.h"

UBTTask_MonsterSkill::UBTTask_MonsterSkill()
{
	NodeName = TEXT("MonsterSkill");
	bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_MonsterSkill::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type result = Super::ExecuteTask(OwnerComp, NodeMemory);

	AMonsterAIController* AI = Cast<AMonsterAIController>(OwnerComp.GetAIOwner());

	if (!IsValid(AI))
		return EBTNodeResult::Failed;

	AMonster* Pawn = Cast<AMonster>(AI->GetPawn());

	if (!IsValid(Pawn))
		return EBTNodeResult::Failed;

	UMonsterAnimInstance* AnimInst = Cast<UMonsterAnimInstance>(Pawn->GetMonsterAnimInst());

	//�����忡 �ִ� Ÿ������ �ҷ�����
	AActor* Target = Cast<AActor>(AI->GetBlackboardComponent()->GetValueAsObject(TEXT("Target")));


	//Ÿ�� ������ ���ҷ����ų�, SkillEnable�� false��� ���� ����
	if (!IsValid(Target) || !AI->GetBlackboardComponent()->GetValueAsBool(TEXT("SkillEnable")))
	{
		AI->StopMovement();

		AnimInst->ChangeAnim(EMonsterAnimType::Idle);

		return EBTNodeResult::Failed;
	}

	// ���������� Target�� SKillEnable üũ�� �Ϸ��ߴٸ�
	// Ÿ���� ��ȿ�����ȿ� �ְ� ��ų ��Ÿ�ӵ� �����ϴٴ� �Ҹ��� ��ų ����

	const FMonsterSkillInfo* Info = Pawn->GetSkillData();
	AnimInst->ChangeAnim(Info->AnimType);



	return EBTNodeResult::InProgress;
}
EBTNodeResult::Type UBTTask_MonsterSkill::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type result = Super::AbortTask(OwnerComp, NodeMemory);
	return result;
}


void UBTTask_MonsterSkill::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	AMonsterAIController* Controller = Cast<AMonsterAIController>(OwnerComp.GetAIOwner());

	if (!IsValid(Controller))
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	AMonster* Monster = Cast<AMonster>(Controller->GetPawn());

	if (!IsValid(Monster))
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	UMonsterAnimInstance* AnimInst = Cast<UMonsterAnimInstance>(Monster->GetMonsterAnimInst());

	ACharacter* Target = Cast<ACharacter>(Controller->GetBlackboardComponent()->GetValueAsObject(TEXT("Target")));
	if (!IsValid(Target))
	{
		Controller->StopMovement();

		AnimInst->ChangeAnim(EMonsterAnimType::Idle);

		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	//������ �������� �Ǵ� üũ �������� �ƴ� ���¿��� ��ų ����ϱ� ����
	if (Monster->GetAttackEnd())
	{
		//������ ������ �ҷ�����
		const FMonsterInfo& Info = Monster->GetMonsterInfo();

		//�ڽŰ� Ÿ���� ��ġ ���� �ҷ�����
		FVector MonsterLoc = Monster->GetActorLocation();
		FVector	TargetLoc = Target->GetActorLocation();

		//������ ���� ��ġ�� ĸ�� �߰��� ����Ű�Ƿ�. �ݸ�ŭ ������.
		MonsterLoc = MonsterLoc - FVector(0.f, 0.f, Monster->GetCapsuleComponent()->GetScaledCapsuleHalfHeight());
		TargetLoc = TargetLoc - FVector(0.f, 0.f, Target->GetCapsuleComponent()->GetScaledCapsuleHalfHeight());

		//�ڽŰ� Ÿ���� �Ÿ� ���ϱ�
		float Distance = FVector::Distance(MonsterLoc, TargetLoc);

		//ĸ�� �ݰ游ŭ�� ����
		Distance -= Monster->GetCapsuleComponent()->GetUnscaledCapsuleRadius();
		Distance -= Target->GetCapsuleComponent()->GetUnscaledCapsuleRadius();

		// ���� ���� ���� �ָ� �ִٸ� ����
		if (Distance > Info.AttackDistance)
		{
			FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		}
		else
		{
			FVector Dir = TargetLoc - MonsterLoc;
			Dir.Z = 0.f;
			Dir.Normalize();


			Monster->SetActorRotation(FRotator(0.f, Dir.Rotation().Yaw, 0.f));
		}
		Monster->SetAttackEnd(false);

	}
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		//FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
}

void UBTTask_MonsterSkill::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult)
{
	Super::OnTaskFinished(OwnerComp, NodeMemory, TaskResult);
}

