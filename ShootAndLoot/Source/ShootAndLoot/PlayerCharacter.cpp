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

	// ī�޶� ����
	PlayerCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("PlayerCamera"));
	PlayerCamera->SetupAttachment(RootComponent);
	PlayerCamera->SetWorldLocation(FVector(0.f, 0.f, 64.f));
	PlayerCamera->bUsePawnControlRotation = true;

	// ���̷�Ż �޽� ����
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SM(TEXT("SkeletalMesh'/Game/FirstPerson/Character/Mesh/SK_Mannequin_Arms.SK_Mannequin_Arms'"));
	if (SM.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SM.Object);
	}
	GetMesh()->SetupAttachment(PlayerCamera);

	// �޽� Transform ����
	//GetMesh()->SetRelativeLocationAndRotation(FVector(10.f, 0.f, -120.f), FRotator(0.f, 0.f, 0.f));
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	// TODO: ���� ���� �����丵
	//// ���� ����_1��
	//auto DefaultWeapon = GetWorld()->SpawnActor<AWeapon>(FVector::ZeroVector, FRotator::ZeroRotator);
	//FName WeaponSocket(TEXT("RightHandSocket"));
	//if (DefaultWeapon)
	//{
	//	DefaultWeapon->AttachToComponent(GetMesh(),
	//		FAttachmentTransformRules::SnapToTargetNotIncludingScale,
	//		WeaponSocket);
	//}

	// ���� ����_2��
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

	// CrosshairLocation ����
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
	// ����Ʈ ������
	FVector2D ViewportSize{ 0.f };
	if (GEngine && GEngine->GameViewport)
	{
		GEngine->GameViewport->GetViewportSize(ViewportSize);
	}

	// �Է� �����¿� �°� CrosshairLocation ����
	CrosshairLocation = FVector2D(ViewportSize.X / 2.f + offsetX, ViewportSize.Y / 2.f + offsetY);
}

bool APlayerCharacter::GetBeamEndLocation(const FVector& BarrelSocketLocation, FVector& BeamEndLocation)
{
	// ���ؼ� ���� ��ġ, ���� ���� ���
	FVector CrosshairWorldPos;
	FVector CrosshairWorldDir;

	bool bScreenToWorld = UGameplayStatics::DeprojectScreenToWorld(
		UGameplayStatics::GetPlayerController(this, 0),
		CrosshairLocation,
		CrosshairWorldPos,
		CrosshairWorldDir);

	if (bScreenToWorld)
	{
		// ���ؼ��� �� ���� ��� (�浹ü�� ������ �� ��������, ������ ������ �ָ�)
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

	// TODO: ���� ���� �߰�
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &APawn::AddControllerPitchInput);

	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &APlayerCharacter::FireWeapon);
}

