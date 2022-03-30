// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "Weapon.generated.h"

/**
 * 
 */

class AProjectile;

UCLASS()
class SHOOTANDLOOT_API AWeapon : public AItem
{
	GENERATED_BODY()

public:
	AWeapon();

private:
	virtual void BeginPlay() override;
	
private:
	/** ����ü Ŭ���� ���� ���� */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AProjectile> CurrentProjectileClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	float AttackInterval = 0.2f;

	float ProjSpeed = 1000.f;

	// ź�� ����
	int32 AmmoCount = 0;
	int32 MagazineCount = 10;
	int32 TotalAmmoCount = 500;
	//int32 MaxAmmoCount = 500;
	float ReloadTime = 1.f;
	// TODO: ���� ź�� �ɼ� �߰�

public:
	FORCEINLINE float GetAttackInterval() const { return AttackInterval; }
	FORCEINLINE float GetReloadTime() const { return ReloadTime; }

public:
	void FireProjectile(const FVector& TargetLocation);

	void Reload();
	bool IsMagazineFull() const { return AmmoCount == MagazineCount; }
};
