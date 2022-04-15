// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAnimInstance.h"

#include "Enemy.h"

UEnemyAnimInstance::UEnemyAnimInstance()
{

}

void UEnemyAnimInstance::UpdateAnimationProperties(float DeltaTime)
{
	if (OwnerCharacter == nullptr)
	{
		OwnerCharacter = Cast<AEnemy>(TryGetPawnOwner());
	}

	if (OwnerCharacter)
	{
		// Speed 업데이트
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

void UEnemyAnimInstance::AnimNotify_EnemyAttackHit()
{
	//UE_LOG(LogTemp, Log, TEXT("AnimNotify_AttackHit"));
	OnAttackHit.Broadcast();
}

void UEnemyAnimInstance::PlayAttackMontage()
{
	Montage_Play(AttackMontage, 1.f);
}

void UEnemyAnimInstance::AttackMontageJumpToSection()
{
	Montage_JumpToSection(FName("Attack"), AttackMontage);
	//UE_LOG(LogTemp, Warning, TEXT("Attack Montage Jump On"));
}
