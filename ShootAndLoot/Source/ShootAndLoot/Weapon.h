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
	virtual void Tick(float DeltaTime) override;

	void RefreshUI();
	
private:
	/** 투사체 클래스 설정 변수 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AProjectile> CurrentProjectileClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	float AttackInterval = 0.2f;

	float ProjSpeed = 1000.f;

	// 탄약 관련
	int32 AmmoCount = 0;
	int32 MagazineCount = 10;
	int32 TotalAmmoCount = 500;
	//int32 MaxAmmoCount = 500;
	float ReloadTime = 1.f;
	// TODO: 무한 탄약 옵션 추가

	// UI 관련
	UPROPERTY()
	TSubclassOf<UUserWidget> UW_Class;

	UPROPERTY()
	UUserWidget* CurrentWidget;

public:
	FORCEINLINE float GetAttackInterval() const { return AttackInterval; }
	FORCEINLINE float GetReloadTime() const { return ReloadTime; }

public:
	void FireProjectile(const FVector& TargetLocation);

	void Reload();
	bool IsMagazineFull() const { return AmmoCount == MagazineCount; }
};
