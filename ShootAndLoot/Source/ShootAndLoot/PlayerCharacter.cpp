// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "Camera/CameraComponent.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 카메라 설정
	PlayerCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("PlayerCamera"));
	PlayerCamera->SetupAttachment(RootComponent);
	PlayerCamera->SetWorldLocation(FVector(0.f, 0.f, 64.f));

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

}

