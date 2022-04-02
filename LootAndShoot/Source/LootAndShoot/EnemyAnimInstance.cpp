// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAnimInstance.h"

#include "Enemy.h"

void UEnemyAnimInstance::UpdateAnimationProperties(float DeltaTime)
{
	if (OwnerCharacter == nullptr)
	{
		OwnerCharacter = Cast<AEnemy>(TryGetPawnOwner());
	}

	if (OwnerCharacter)
	{
		// Speed ������Ʈ
		FVector Velocity{ OwnerCharacter->GetVelocity() };
		Velocity.Z = 0.f;
		Speed = Velocity.Size();
	}
}

void UEnemyAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	OwnerCharacter = Cast<AEnemy>(TryGetPawnOwner());
}
