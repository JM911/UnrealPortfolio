// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include "Projectile.h"
#include "Engine/SkeletalMeshSocket.h"
#include "GameFramework/ProjectileMovementComponent.h"

AWeapon::AWeapon()
{

}

void AWeapon::BeginPlay()
{
	Super::BeginPlay();

	Reload();
}

void AWeapon::Reload()
{
	int32 DiffBetweenAmmoAndMag = MagazineCount - AmmoCount;

	AmmoCount = MagazineCount;
	TotalAmmoCount -= DiffBetweenAmmoAndMag;
}

void AWeapon::FireProjectile(const FVector& TargetLocation)
{
	const USkeletalMeshSocket* BarrelSocket = GetItemMesh()->GetSocketByName(FName("Muzzle"));

	// 탄약 체크
	if (AmmoCount <= 0)
		return;

	if (BarrelSocket && CurrentProjectileClass)
	{
		const FTransform SocketTransform = BarrelSocket->GetSocketTransform(GetItemMesh());
		AProjectile* CurProjectile = GetWorld()->SpawnActor<AProjectile>(
			CurrentProjectileClass,
			SocketTransform);

		FVector ProjDirection = (TargetLocation - SocketTransform.GetLocation()).GetSafeNormal();
		FVector ProjVelocity = ProjDirection * ProjSpeed;

		CurProjectile->GetProjMove()->Velocity = ProjVelocity;

		// 탄약 업데이트
		AmmoCount--;
	}
}
