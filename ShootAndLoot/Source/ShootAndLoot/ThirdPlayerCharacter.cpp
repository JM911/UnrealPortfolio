// Fill out your copyright notice in the Description page of Project Settings.


#include "ThirdPlayerCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

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

	// 카메라 설정
	PlayerCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("PlayerCamera"));
	PlayerCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	PlayerCamera->bUsePawnControlRotation = false;

	//// Don't rotate when the controller rotates. Let the controller only affect the camera.
	//bUseControllerRotationPitch = false;
	//bUseControllerRotationYaw = true;
	//bUseControllerRotationRoll = false;
}

// Called when the game starts or when spawned
void AThirdPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void AThirdPlayerCharacter::MoveForward(float Value)
{
	if (Controller)
	{
		const FRotator Rotation{ Controller->GetControlRotation() };
		const FRotator YawRotation{ 0.f, Rotation.Yaw, 0.f };
		const FVector Direction{ FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X) };
		AddMovementInput(Direction, Value);

		// 이동 방향 변수 업데이트
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

		// 이동 방향 변수 업데이트
		LeftRightValue = Value;
	}
}

// Called every frame
void AThirdPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AThirdPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AThirdPlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AThirdPlayerCharacter::MoveRight);

	// TODO: 감도 설정 추가
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &APawn::AddControllerPitchInput);

	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Released, this, &ACharacter::StopJumping);

}

