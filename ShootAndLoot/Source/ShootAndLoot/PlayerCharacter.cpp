// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "Weapon.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Kismet/GameplayStatics.h"
#include "Projectile.h"
#include "Engine/World.h"

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
		//UE_LOG(LogTemp, Warning, TEXT("Spawn Weapon"));
	}
	const USkeletalMeshSocket* RightHandSocket = GetMesh()->GetSocketByName(FName("RightHandSocket"));
	if (RightHandSocket && DefaultWeapon)
	{
		RightHandSocket->AttachActor(DefaultWeapon, GetMesh());
		//UE_LOG(LogTemp, Warning, TEXT("Attach Weapon"));
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

bool APlayerCharacter::GetBeamEndLocation(const FVector& BarrelSocketLocation, FVector& BeamEndLocation)
{
	// 조준선 월드 위치, 월드 방향 계산
	FVector CrosshairWorldPos;
	FVector CrosshairWorldDir;

	bool bScreenToWorld = UGameplayStatics::DeprojectScreenToWorld(
		UGameplayStatics::GetPlayerController(this, 0),
		CrosshairLocation,
		CrosshairWorldPos,
		CrosshairWorldDir);

	if (bScreenToWorld)
	{
		// 조준선의 끝 지점 계산 (충돌체가 있으면 그 지점으로, 없으면 적당히 멀리)
		const FVector Start{ CrosshairWorldPos };
		const FVector End{ Start + CrosshairWorldDir * 50'000.f };

		// TODO
	}

	return false;
}

void APlayerCharacter::FireWeapon()
{
	const USkeletalMeshSocket* BarrelSocket = GetMesh()->GetSocketByName(FName("BarrelSocket"));
	if (BarrelSocket)
	{
		// Test
		const FTransform SocketTransform = BarrelSocket->GetSocketTransform(GetMesh());
		UWorld* World = GetWorld();
		if (World && CurrentProjectileClass)
		{
			World->SpawnActor<AProjectile>(
				CurrentProjectileClass,
				SocketTransform);
		}
	}

}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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

	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &APlayerCharacter::FireWeapon);
}

