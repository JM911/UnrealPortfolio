// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"

// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ProjMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjMesh"));
	SetRootComponent(ProjMesh);
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	
	// 투사체는 중력영향 없이 일직선으로 발사
	ProjMesh->SetEnableGravity(false);
	ProjMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

