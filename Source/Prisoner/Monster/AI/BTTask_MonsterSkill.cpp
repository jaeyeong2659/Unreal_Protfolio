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

	//블랙보드에 있는 타겟정보 불러오기
	AActor* Target = Cast<AActor>(AI->GetBlackboardComponent()->GetValueAsObject(TEXT("Target")));


	//타겟 정보를 못불러오거나, SkillEnable이 false라면 동작 정지
	if (!IsValid(Target) || !AI->GetBlackboardComponent()->GetValueAsBool(TEXT("SkillEnable")))
	{
		AI->StopMovement();

		AnimInst->ChangeAnim(EMonsterAnimType::Idle);

		return EBTNodeResult::Failed;
	}

	// 정상적으로 Target과 SKillEnable 체크를 완료했다면
	// 타겟이 유효범위안에 있고 스킬 쿨타임도 가능하다는 소리니 스킬 동작

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

	//공격이 끝났는지 판단 체크 공격중이 아닌 상태에서 스킬 사용하기 위함
	if (Monster->GetAttackEnd())
	{
		//몬스터의 정보를 불러오기
		const FMonsterInfo& Info = Monster->GetMonsterInfo();

		//자신과 타겟의 위치 정보 불러오기
		FVector MonsterLoc = Monster->GetActorLocation();
		FVector	TargetLoc = Target->GetActorLocation();

		//몬스터의 현재 위치는 캡슐 중간을 가리키므로. 반만큼 내리기.
		MonsterLoc = MonsterLoc - FVector(0.f, 0.f, Monster->GetCapsuleComponent()->GetScaledCapsuleHalfHeight());
		TargetLoc = TargetLoc - FVector(0.f, 0.f, Target->GetCapsuleComponent()->GetScaledCapsuleHalfHeight());

		//자신과 타겟의 거리 구하기
		float Distance = FVector::Distance(MonsterLoc, TargetLoc);

		//캡슐 반경만큼도 제거
		Distance -= Monster->GetCapsuleComponent()->GetUnscaledCapsuleRadius();
		Distance -= Target->GetCapsuleComponent()->GetUnscaledCapsuleRadius();

		// 공격 범위 보다 멀리 있다면 종료
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

