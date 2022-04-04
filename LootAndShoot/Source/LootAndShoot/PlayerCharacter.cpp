// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"

#include "Camera/CameraComponent.h"

#include "Projectile.h"

#include "MyGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerHUD.h"
#include "Components/TextBlock.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	// 카메라 설정
	FirstPersonCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("First Person Camera"));
	//FirstPersonCamera->SetupAttachment(GetCapsuleComponent());
	FirstPersonCamera->SetupAttachment(RootComponent);
	FirstPersonCamera->bUsePawnControlRotation = true;

	// 손 메쉬 설정
	HandsMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Character Mesh"));
	HandsMesh->SetOnlyOwnerSee(true);
	HandsMesh->SetupAttachment(FirstPersonCamera);
	HandsMesh->bCastDynamicShadow = false;
	HandsMesh->CastShadow = false;

	// 총 메쉬 설정
	GunMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Gun"));
	GunMesh->SetOnlyOwnerSee(true);
	GunMesh->bCastDynamicShadow = false;
	GunMesh->CastShadow = false;

	// 투사체 발사 위치 저장용 컴포넌트 설정
	MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("Muzzle Location"));
	MuzzleLocation->SetupAttachment(GunMesh);

}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	// 총 메쉬 장착
	GunMesh->AttachToComponent(HandsMesh,
		FAttachmentTransformRules::SnapToTargetNotIncludingScale,
		TEXT("GripPoint"));

	// 재장전
	Reload();
	
}

void APlayerCharacter::MoveForward(float Value)
{
	if (Value != 0.f)
	{
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void APlayerCharacter::MoveRight(float Value)
{
	if (Value != 0.f)
	{
		AddMovementInput(GetActorRightVector(), Value);
	}
}

void APlayerCharacter::FireProjectile()
{
	// TODO: 나중에 조준점으로 발사하도록 변경?

	UWorld* World = GetWorld();
	if (World && MuzzleLocation)
	{
		FRotator SpawnRotation = GetControlRotation();
		FVector SpawnLocation = MuzzleLocation->GetComponentLocation();

		FActorSpawnParameters ActorSpawnParams;
		ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

		World->SpawnActor<AProjectile>(ProjectileClass,
			SpawnLocation, SpawnRotation, ActorSpawnParams);

		// TODO: 사운드, 애니메이션 설정
	}
}

void APlayerCharacter::FireButtonPressed()
{
	if (bReloading)
		return;

	bFireButtonPressed = true;
}

void APlayerCharacter::FireButtonReleased()
{
	bFireButtonPressed = false;
}

void APlayerCharacter::FireNextReady()
{
	bFireReady = true;
}

void APlayerCharacter::FireFinal()
{
	if (bFireButtonPressed && bFireReady)
	{
		// 잔탄 체크
		if (CurrentBullet <= 0)
			return;

		bFireReady = false;
		FireProjectile();
		GetWorldTimerManager().SetTimer(
			FireTimer,
			this,
			&APlayerCharacter::FireNextReady,
			FireInterval);

		// 잔탄 업데이트
		CurrentBullet--;

		// 디버그용
		//UE_LOG(LogTemp, Warning, TEXT("%d / %d , %d"), CurrentBullet, MagazineBullet, TotalBullet);
	}
}

void APlayerCharacter::Reload()
{
	if (bReloading || CurrentBullet == MagazineBullet)
		return;

	bReloading = true;

	GetWorldTimerManager().SetTimer(
		ReloadTimer,
		this,
		&APlayerCharacter::ReloadEnd,
		ReloadTime);
}

void APlayerCharacter::ReloadEnd()
{
	bReloading = false;

	// 실제 장전
	int32 BulletDiff = MagazineBullet - CurrentBullet;
	CurrentBullet = MagazineBullet;
	TotalBullet -= BulletDiff;

	// 디버그용
	//UE_LOG(LogTemp, Warning, TEXT("Realod Complete!"));
}

void APlayerCharacter::UpdatePlayerHUD()
{
	AMyGameMode* CurGameMode = Cast<AMyGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

	if (CurGameMode)
	{
		UPlayerHUD* CurHUD = Cast<UPlayerHUD>(CurGameMode->GetCurrentWidget());
		if (CurHUD)
		{
			const FString AmmoStr = FString::Printf(TEXT("Ammo %02d/%02d\nTotal %03d"),
				CurrentBullet, MagazineBullet, TotalBullet);

			CurHUD->GetAmmoText()->SetText(FText::FromString(AmmoStr));
		}
	}
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 투사체 연사 체크
	FireFinal();
	
	// 탄약 관련 UI 업데이트
	UpdatePlayerHUD();
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &APlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APlayerCharacter::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &ACharacter::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &ACharacter::AddControllerPitchInput);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &APlayerCharacter::FireButtonPressed);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &APlayerCharacter::FireButtonReleased);

	PlayerInputComponent->BindAction("Reload", IE_Pressed, this, &APlayerCharacter::Reload);
}

