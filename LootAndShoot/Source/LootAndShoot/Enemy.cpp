// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"

#include "Components/CapsuleComponent.h"
#include "Components/BoxComponent.h"
#include "Projectile.h"

#include "Kismet/KismetMathLibrary.h"

#include "EnemyAnimInstance.h"


// Sets default values
AEnemy::AEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 캡슐 충돌체 크기 설정
	GetCapsuleComponent()->InitCapsuleSize(34.f, 80.f);

	// 메인 바디 메쉬 설정
	USkeletalMeshComponent* MainBodyMesh = GetMesh();
	MainBodyMesh->SetRelativeLocation(FVector(0.f, 0.f, -75.f));
	MainBodyMesh->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));

	// 기타 추가 메쉬 설정
	Weapon_R = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Weapon_R"));
	Weapon_R->SetupAttachment(MainBodyMesh, "Weapon_Socket_R");

	Weapon_L = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Weapon_L"));
	Weapon_L->SetupAttachment(MainBodyMesh, "Weapon_Socket_L");

	Shield = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Shield"));
	Shield->SetupAttachment(MainBodyMesh, "Shield_Socket");

	BackPack = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BackPack"));
	BackPack->SetupAttachment(MainBodyMesh, "BackPack_Socket");

	// 머리 메쉬 설정
	Head = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Head"));
	Head->SetupAttachment(MainBodyMesh);


	// 충돌 박스 설정
	DamageCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Damage Collision Box"));
	DamageCollisionBox->SetupAttachment(MainBodyMesh);
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	// 충돌 함수 설정
	DamageCollisionBox->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::OnHit);

	// 애님 인스턴스 및 몽타주 설정
	AnimInstanceBody = Cast<UEnemyAnimInstance>(GetMesh()->GetAnimInstance());
	if (AnimInstanceBody)
	{
		AnimInstanceBody->OnMontageEnded.AddDynamic(this, &AEnemy::OnAnimMontageEnded);
	}

	AnimInstanceHead = Cast<UEnemyAnimInstance>(Head->GetAnimInstance());
	//if (AnimInstanceHead)
	//{
	//	AnimInstanceHead->OnMontageEnded.AddDynamic(this, &AEnemy::OnAnimMontageEnded);
	//}
	
}

//void AEnemy::PostInitializeComponents()
//{
//	Super::PostInitializeComponents();
//}

void AEnemy::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit)
{
	AProjectile* Projectile = Cast<AProjectile>(OtherActor);

	if (Projectile)//&& OtherActor->ActorHasTag("Projectile"))	// 굳이 둘 다 체크해야 할까?
	{
		// 리팩토링 고려...
		CurrentHp -= Projectile->GetDamageValue();
		
		if (CurrentHp <= 0.f) //UKismetMathLibrary::NearlyEqual_FloatFloat(CurrentHp, 0.f))
		{
			Destroy();
		}
	}
}

void AEnemy::Attack()
{
	if (bIsAttacking)
		return;

	AnimInstanceBody->PlayAttackMontage();
	AnimInstanceHead->PlayAttackMontage();
	AnimInstanceBody->AttackMontageJumpToSection();
	AnimInstanceHead->AttackMontageJumpToSection();
	bIsAttacking = true;
}

void AEnemy::OnAnimMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (bIsAttacking)
	{
		bIsAttacking = false;
		OnAttackEnd.Broadcast();
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

