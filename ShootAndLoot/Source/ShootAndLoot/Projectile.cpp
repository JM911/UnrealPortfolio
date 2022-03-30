// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ProjMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjMesh"));
	SetRootComponent(ProjMesh);

	ProjMove = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjMove"));
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

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

