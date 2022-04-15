// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Enemy.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnAttackEnd);

UCLASS()
class LOOTANDSHOOT_API AEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//virtual void PostInitializeComponents() override;


	// �ǰ� �Լ�(�浹)
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit);

	// ������ ���
	void DoDropItem();


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


private:
	// �޽���
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* Weapon_R;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* Weapon_L;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* Shield;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* BackPack;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* Head;


	// �浹 ���� => ���� ������ ��������
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Collision, meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* DamageCollisionBox;


	// ���� ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat, meta = (AllowPrivateAccess = "true"))
	float CurrentHp = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat, meta = (AllowPrivateAccess = "true"))
	float MaxHp = 100.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Stat, meta = (AllowPrivateAccess = "true"))
	float AttackPoint = 10.f;

	UPROPERTY()
	class UEnemyAnimInstance* AnimInstanceBody;

	UPROPERTY()
	class UEnemyAnimInstance* AnimInstanceHead;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true"))
	bool bIsAttacking = false;


	// ������ ��� ����
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Item, meta = (AllowPrivateAccess = "true"))
	TArray<TSubclassOf<class AItem>> DropItemClassArray;


	// �Ÿ� ���� üũ�� ����
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Distance, meta = (AllowPrivateAccess = "true"))
	float CanAttackDistance = 200.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Distance, meta = (AllowPrivateAccess = "true"))
	float SearchDistance = 500.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Distance, meta = (AllowPrivateAccess = "true"))
	float WidgetVisibleDistance = 1000.f;

public:
	// �Ÿ� ���� Get
	float GetCanAttackDistance() const { return CanAttackDistance; }
	float GetSearchDistance() const { return SearchDistance; }

	// ����
	void Attack();
	FOnAttackEnd OnAttackEnd;

	// �ִϸ��̼�
	UFUNCTION()
	void OnAnimMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	UFUNCTION()
	void AttackCheck();
};
