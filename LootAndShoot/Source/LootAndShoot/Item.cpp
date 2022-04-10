// Fill out your copyright notice in the Description page of Project Settings.


#include "Item.h"

#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
AItem::AItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// 바닥 충돌용 박스 설정
	FloorCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Floor Collider"));
	SetRootComponent(FloorCollision);

	// 충돌 구체 설정
	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Collider"));
	SphereCollision->SetupAttachment(RootComponent);
	SphereCollision->InitSphereRadius(30.f);

	// 메쉬 설정
	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Item Mesh"));
	ItemMesh->SetupAttachment(RootComponent);

	// 아이템 움직임 설정
	ItemMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement"));
	ItemMovement->UpdatedComponent = FloorCollision;
	ItemMovement->bRotationFollowsVelocity = false;
}

// Called when the game starts or when spawned
void AItem::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

