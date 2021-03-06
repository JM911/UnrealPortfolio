// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "EnemyAnimInstance.generated.h"

/**
 * 
 */
// 공격 판정 관련 델리게이트
DECLARE_MULTICAST_DELEGATE(FOnAttackHit);

UCLASS()
class LOOTANDSHOOT_API UEnemyAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UEnemyAnimInstance();

protected:
	UFUNCTION(BlueprintCallable)
	void UpdateAnimationProperties(float DeltaTime);

	virtual void NativeInitializeAnimation() override;


private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class AEnemy* OwnerCharacter;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	float Speed;

	// 공격 애니메이션 몽타주 (블프에서 설정)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Combat, Meta = (AllowPrivateAccess = true))
	UAnimMontage* AttackMontage;

	// 공격 판정 관련 애님 노티파이
	UFUNCTION()
	void AnimNotify_EnemyAttackHit();

		
public:
	void PlayAttackMontage();
	void AttackMontageJumpToSection();

	FOnAttackHit OnAttackHit;
};
