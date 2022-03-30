// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "Weapon.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Kismet/GameplayStatics.h"
#include "Projectile.h"
#include "Engine/World.h"

#include "ShootAndLootGameModeBase.h"
#include "MyHUD.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 카메라 설정
	PlayerCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("PlayerCamera"));
	PlayerCamera->SetupAttachment(RootComponent);
	PlayerCamera->SetWorldLocation(FVector(0.f, 0.f, 64.f));
	PlayerCamera->bUsePawnControlRotation = true;

	// 스켈레탈 메쉬 설정
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SM(TEXT("SkeletalMesh'/Game/FirstPerson/Character/Mesh/SK_Mannequin_Arms.SK_Mannequin_Arms'"));
	if (SM.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SM.Object);
	}
	GetMesh()->SetupAttachment(PlayerCamera);

	// 메쉬 Transform 설정
	//GetMesh()->SetRelativeLocationAndRotation(FVector(10.f, 0.f, -120.f), FRotator(0.f, 0.f, 0.f));
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	// 카메라 기본 거리 설정
	if (PlayerCamera)
	{
		CameraDefaultFOV = PlayerCamera->FieldOfView;
		CameraCurrentFOV = CameraDefaultFOV;
	}

	// TODO: 무기 착용 리팩토링
	//// 무기 착용_1안
	//auto DefaultWeapon = GetWorld()->SpawnActor<AWeapon>(FVector::ZeroVector, FRotator::ZeroRotator);
	//FName WeaponSocket(TEXT("RightHandSocket"));
	//if (DefaultWeapon)
	//{
	//	DefaultWeapon->AttachToComponent(GetMesh(),
	//		FAttachmentTransformRules::SnapToTargetNotIncludingScale,
	//		WeaponSocket);
	//}

	// 무기 착용_2안
	AWeapon* DefaultWeapon = nullptr;
	{
		DefaultWeapon = GetWorld()->SpawnActor<AWeapon>(DefaultWeaponClass);
		CurWeapon = DefaultWeapon;
	}
	const USkeletalMeshSocket* RightHandSocket = GetMesh()->GetSocketByName(FName("RightHandSocket"));
	if (RightHandSocket && DefaultWeapon)
	{
		RightHandSocket->AttachActor(DefaultWeapon, GetMesh());
	}

	// CrosshairLocation 설정
	SetCrosshairLocation(0.f, 0.f);
}

void APlayerCharacter::MoveForward(float Value)
{
	if (Controller)
	{
		const FRotator Rotation{ Controller->GetControlRotation() };
		const FRotator YawRotation{ 0.f, Rotation.Yaw, 0.f };
		const FVector Direction{ FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X) };
		AddMovementInput(Direction, Value);
	}
}
void APlayerCharacter::MoveRight(float Value)
{
	if (Controller)
	{
		const FRotator Rotation{ Controller->GetControlRotation() };
		const FRotator YawRotation{ 0.f, Rotation.Yaw, 0.f };
		const FVector Direction{ FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y) };
		AddMovementInput(Direction, Value);
	}
}

void APlayerCharacter::SetCrosshairLocation(float offsetX, float offsetY)
{
	// 뷰포트 사이즈
	FVector2D ViewportSize{ 0.f };
	if (GEngine && GEngine->GameViewport)
	{
		GEngine->GameViewport->GetViewportSize(ViewportSize);
	}

	// 입력 오프셋에 맞게 CrosshairLocation 설정
	CrosshairLocation = FVector2D(ViewportSize.X / 2.f + offsetX, ViewportSize.Y / 2.f + offsetY);
}

void APlayerCharacter::FireButtonPressed()
{
	bIsAttacking = true;
	StartFireTimer();
}

void APlayerCharacter::FireButtonReleased()
{
	bIsAttacking = false;
}

void APlayerCharacter::StartFireTimer()
{
	if (bFireEnable && CurWeapon)
	{
		FireWeapon();
		bFireEnable = false;
		GetWorldTimerManager().SetTimer(
			AutoFireTimer,
			this,
			&APlayerCharacter::AutoFireReset,
			CurWeapon->GetAttackInterval());
	}
}

void APlayerCharacter::AutoFireReset()
{
	bFireEnable = true;
	if (bIsAttacking)
	{
		StartFireTimer();
	}
}

void APlayerCharacter::FireWeapon()
{
	// TODO: 조준선 위치 구해서 대입
	FVector AimTargetLocation{ 0.f };
	FHitResult OutHitResult;

	TraceUnderCrosshair(OutHitResult, AimTargetLocation);
	CurWeapon->FireProjectile(AimTargetLocation);
}

bool APlayerCharacter::TraceUnderCrosshair(FHitResult& OutHitResult, FVector& OutHitLocation)
{
	FVector CrosshairWorldPosition;
	FVector CrosshairWorldDirecton;

	bool bScreenToWorld = UGameplayStatics::DeprojectScreenToWorld(
		UGameplayStatics::GetPlayerController(this, 0),
		CrosshairLocation,
		CrosshairWorldPosition,
		CrosshairWorldDirecton);

	if (bScreenToWorld)
	{
		// Trace from Crosshair world location outward
		const FVector Start{ CrosshairWorldPosition };
		const FVector End{ Start + CrosshairWorldDirecton * 50'000.f };
		OutHitLocation = End;
		GetWorld()->LineTraceSingleByChannel(
			OutHitResult,
			Start,
			End,
			ECollisionChannel::ECC_Visibility);
		if (OutHitResult.bBlockingHit)
		{
			OutHitLocation = OutHitResult.Location;
			return true;
		}
	}

	return false;
}

void APlayerCharacter::AimButtonPressed()
{
	bAiming = true;
}

void APlayerCharacter::AimButtonReleased()
{
	bAiming = false;
}

void APlayerCharacter::AimingCameraZoom(float DeltaTime)
{
	if (bAiming)
	{
		CameraCurrentFOV = FMath::FInterpTo(
			CameraCurrentFOV,
			CameraZoomedFOV,
			DeltaTime,
			ZoomInterpSpeed);
	}
	else
	{
		CameraCurrentFOV = FMath::FInterpTo(
			CameraCurrentFOV,
			CameraDefaultFOV,
			DeltaTime,
			ZoomInterpSpeed);
	}
	if (PlayerCamera)
	{
		PlayerCamera->SetFieldOfView(CameraCurrentFOV);
	}
}

void APlayerCharacter::Reload()
{
	if (bIsReloading)
		return;

	if (CurWeapon)
	{
		if (CurWeapon->IsMagazineFull())
			return;

		bIsReloading = true;
		bFireEnable = false;
		GetWorldTimerManager().SetTimer(
			ReloadTimer,
			this,
			&APlayerCharacter::ReloadEnd,
			CurWeapon->GetReloadTime());
	}
}

void APlayerCharacter::ReloadEnd()
{
	bFireEnable = true;
	bIsReloading = false;
	CurWeapon->Reload();
}

void APlayerCharacter::TempRefreshUI()
{

}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 조준 버튼 체크 및 카메라 설정
	AimingCameraZoom(DeltaTime);
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &APlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &APlayerCharacter::MoveRight);

	// TODO: 감도 설정 추가
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &APawn::AddControllerPitchInput);

	PlayerInputComponent->BindAction(TEXT("Aim"), IE_Pressed, this, &APlayerCharacter::AimButtonPressed);
	PlayerInputComponent->BindAction(TEXT("Aim"), IE_Released, this, &APlayerCharacter::AimButtonReleased);

	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &APlayerCharacter::FireButtonPressed);
	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Released, this, &APlayerCharacter::FireButtonReleased);

	PlayerInputComponent->BindAction(TEXT("Reload"), IE_Pressed, this, &APlayerCharacter::Reload);
}

