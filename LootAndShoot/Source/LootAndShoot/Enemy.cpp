// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"

#include "Components/CapsuleComponent.h"
#include "Components/BoxComponent.h"
#include "Projectile.h"

#include "Kismet/KismetMathLibrary.h"


// Sets default values
AEnemy::AEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// ĸ�� �浹ü ũ�� ����
	GetCapsuleComponent()->InitCapsuleSize(34.f, 80.f);

	// ���� �ٵ� �޽� ����
	USkeletalMeshComponent* MainBodyMesh = GetMesh();
	MainBodyMesh->SetRelativeLocation(FVector(0.f, 0.f, -75.f));
	MainBodyMesh->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));

	// ��Ÿ �߰� �޽� ����
	Weapon_R = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Weapon_R"));
	Weapon_R->SetupAttachment(MainBodyMesh, "Weapon_Socket_R");

	Weapon_L = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Weapon_L"));
	Weapon_L->SetupAttachment(MainBodyMesh, "Weapon_Socket_L");

	Shield = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Shield"));
	Shield->SetupAttachment(MainBodyMesh, "Shield_Socket");

	BackPack = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BackPack"));
	BackPack->SetupAttachment(MainBodyMesh, "BackPack_Socket");

	// �Ӹ� �޽� ����
	Head = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Head"));
	Head->SetupAttachment(MainBodyMesh);


	// �浹 �ڽ� ����
	DamageCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Damage Collision Box"));
	DamageCollisionBox->SetupAttachment(MainBodyMesh);
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	// �浹 �Լ� ����
	DamageCollisionBox->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::OnHit);
	
}

//void AEnemy::PostInitializeComponents()
//{
//	Super::PostInitializeComponents();
//}

void AEnemy::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit)
{
	AProjectile* Projectile = Cast<AProjectile>(OtherActor);

	if (Projectile)//&& OtherActor->ActorHasTag("Projectile"))	// ���� �� �� üũ�ؾ� �ұ�?
	{
		// �����丵 ���...
		CurrentHp -= Projectile->GetDamageValue();
		
		if (UKismetMathLibrary::NearlyEqual_FloatFloat(CurrentHp, 0.f))
		{
			Destroy();
		}
	}
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

