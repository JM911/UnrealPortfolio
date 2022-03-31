// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"

// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// �浹ü ����
	CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));
	CollisionSphere->BodyInstance.SetCollisionProfileName(TEXT("Projectile"));
	CollisionSphere->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);

	CollisionSphere->InitSphereRadius(15.f);
	RootComponent = CollisionSphere;

	// ����ƽ �޽�
	ProjMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjMesh"));
	//SetRootComponent(ProjMesh);
	ProjMesh->AttachTo(RootComponent);

	// ����ü �
	ProjMove = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjMove"));

	// ���� ����
	InitialLifeSpan = 5.f;
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	
	// ����ü�� �߷¿��� ���� ���������� �߻�
	//ProjMesh->SetEnableGravity(false);
	ProjMove->ProjectileGravityScale = 0.f;
	ProjMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor != this)
	{
		if (OtherActor->ActorHasTag(FName("Enemy")))
		{
			UE_LOG(LogTemp, Warning, TEXT("Collision With Enemy"));
		}

		Destroy();
	}
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

