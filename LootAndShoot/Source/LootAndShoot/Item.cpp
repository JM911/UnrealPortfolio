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

	// �ٴ� �浹�� �ڽ� ����
	FloorCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Floor Collider"));
	SetRootComponent(FloorCollision);

	// �浹 ��ü ����
	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Collider"));
	SphereCollision->SetupAttachment(RootComponent);
	SphereCollision->InitSphereRadius(30.f);

	// �޽� ����
	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Item Mesh"));
	ItemMesh->SetupAttachment(RootComponent);

	// ������ ������ ����
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

