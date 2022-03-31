// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include "Projectile.h"
#include "Engine/SkeletalMeshSocket.h"
#include "GameFramework/ProjectileMovementComponent.h"

#include "MyUserWidget.h"
#include "Components/TextBlock.h"

AWeapon::AWeapon()
{
	PrimaryActorTick.bCanEverTick = true;

	// User Widget 로드 & 뷰포트 추가
	static ConstructorHelpers::FClassFinder<UMyUserWidget> TestAmmoUI(TEXT("WidgetBlueprint'/Game/_ShootAndLoot/UI/WBP_TestUI.WBP_TestUI_C'"));
	if (TestAmmoUI.Succeeded())
	{
		UW_Class = TestAmmoUI.Class;
		CurrentWidget = CreateWidget(GetWorld(), UW_Class);
		if (CurrentWidget)
		{
			CurrentWidget->AddToViewport();
		}
	}
}

void AWeapon::BeginPlay()
{
	Super::BeginPlay();

	Reload();
	RefreshUI();
}

void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AWeapon::RefreshUI()
{
	UMyUserWidget* CurMyWidget = Cast<UMyUserWidget>(CurrentWidget);
	if (CurMyWidget)
	{
		const FString AmmoStr = FString::Printf(TEXT("Ammo %02d / %02d\nTotal Ammo %03d"),
			AmmoCount, MagazineCount, TotalAmmoCount);
		CurMyWidget->AmmoText->SetText(FText::FromString(AmmoStr));
	}
}

void AWeapon::Reload()
{
	int32 DiffBetweenAmmoAndMag = MagazineCount - AmmoCount;

	AmmoCount = MagazineCount;
	TotalAmmoCount -= DiffBetweenAmmoAndMag;

	RefreshUI();
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

	RefreshUI();
}
