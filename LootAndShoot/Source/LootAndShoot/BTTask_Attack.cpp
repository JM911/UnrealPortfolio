// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Attack.h"
#include "EnemyAIController.h"
#include "Enemy.h"

UBTTask_Attack::UBTTask_Attack()
{
	bNotifyTick = true;
	NodeName = TEXT("Attack");
}

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto MyCharacter = Cast<AEnemy>(OwnerComp.GetAIOwner()->GetPawn());
	if (MyCharacter == nullptr)
		return EBTNodeResult::Failed;

	MyCharacter->Attack();
	bIsAttacking = true;

	MyCharacter->OnAttackEnd.AddLambda([this]()
		{
			bIsAttacking = false;
		});

	return Result;
}

void UBTTask_Attack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	if (bIsAttacking == false)
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
}
