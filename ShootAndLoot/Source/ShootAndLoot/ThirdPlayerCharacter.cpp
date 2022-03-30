// Fill out your copyright notice in the Description page of Project Settings.


#include "ThirdPlayerCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Weapon.h"

// Sets default values
AThirdPlayerCharacter::AThirdPlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create a camera boom (pulls in towards the character if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 180.f;
	CameraBoom->bUsePawnControlRotation = true;
	CameraBoom->SocketOffset = FVector(0.f, 50.f, 70.f);

	// ī�޶� ����
	PlayerCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("PlayerCamera"));
	PlayerCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	PlayerCamera->bUsePawnControlRotation = false;

	// Don't rotate when the controller rotates. Let the controller only affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;
}

// Called when the game starts or when spawned
void AThirdPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	// ī�޶� �⺻ �Ÿ� ����
	if (PlayerCamera)
	{
		CameraDefaultFOV = PlayerCamera->FieldOfView;
		CameraCurrentFOV = CameraDefaultFOV;
	}
	
	// TODO: �Լ��� �����丵
	AWeapon* DefaultWeapon = nullptr;
	{
		DefaultWeapon = GetWorld()->SpawnActor<AWeapon>(DefaultWeaponClass);
		//UE_LOG(LogTemp, Warning, TEXT("Spawn Weapon"));
		CurWeapon = DefaultWeapon;
	}
	const USkeletalMeshSocket* RightHandSocket = GetMesh()->GetSocketByName(FName("RightHandSocket"));
	if (RightHandSocket && DefaultWeapon)
	{
		RightHandSocket->AttachActor(DefaultWeapon, GetMesh());
		//UE_LOG(LogTemp, Warning, TEXT("Attach Weapon"));
	}
}

void AThirdPlayerCharacter::MoveForward(float Value)
{
	if (Controller)
	{
		const FRotator Rotation{ Controller->GetControlRotation() };
		const FRotator YawRotation{ 0.f, Rotation.Yaw, 0.f };
		const FVector Direction{ FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X) };
		AddMovementInput(Direction, Value);

		// �̵� ���� ���� ������Ʈ
		UpDownValue = Value;
	}
}

void AThirdPlayerCharacter::MoveRight(float Value)
{
	if (Controller)
	{
		const FRotator Rotation{ Controller->GetControlRotation() };
		const FRotator YawRotation{ 0.f, Rotation.Yaw, 0.f };
		const FVector Direction{ FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y) };
		AddMovementInput(Direction, Value);

		// �̵� ���� ���� ������Ʈ
		LeftRightValue = Value;
	}
}

void AThirdPlayerCharacter::IronSightPressed()
{
	bIronSight = true;
}

void AThirdPlayerCharacter::IronSightReleased()
{
	bIronSight = false;
}

void AThirdPlayerCharacter::IronSightCameraZoom(float DeltaTime)
{
	if (bIronSight)
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

void AThirdPlayerCharacter::FireWeapon()
{
	// TODO: ���� �ϼ�

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && FireMontage)
	{
		UE_LOG(LogTemp, Warning, TEXT("asdf"));
		AnimInstance->Montage_Play(FireMontage);
		AnimInstance->Montage_JumpToSection(FName("StartFire"));
	}
}

// Called every frame
void AThirdPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// �� ƽ���� ���� üũ, ī�޶� ����
	IronSightCameraZoom(DeltaTime);

}

// Called to bind functionality to input
void AThirdPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AThirdPlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AThirdPlayerCharacter::MoveRight);

	// TODO: ���� ���� �߰�
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &APawn::AddControllerPitchInput);

	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAction(TEXT("IronSight"), IE_Pressed, this, &AThirdPlayerCharacter::IronSightPressed);
	PlayerInputComponent->BindAction(TEXT("IronSight"), IE_Released, this, &AThirdPlayerCharacter::IronSightReleased);

	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &AThirdPlayerCharacter::FireWeapon);
}

