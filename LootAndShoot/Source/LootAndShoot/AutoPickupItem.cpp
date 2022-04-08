// Fill out your copyright notice in the Description page of Project Settings.


#include "AutoPickupItem.h"

#include "Components/SphereComponent.h"

#include "PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"

AAutoPickupItem::AAutoPickupItem()
{
	// ƽ �̺�Ʈ on
	PrimaryActorTick.bCanEverTick = true;

	// �̹� ������ �ִ� �浹 ��ü ũ�� Ű���
	GetSphereCollision()->InitSphereRadius(200.f);
}

void AAutoPickupItem::BeginPlay()
{
	Super::BeginPlay();

	// �÷��̾� �� �ʱ�ȭ
	PlayerPawn = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));

	// �浹 �Լ� ����
	GetSphereCollision()->OnComponentBeginOverlap.AddDynamic(this, &AAutoPickupItem::CollisionCheck);
}

void AAutoPickupItem::CollisionCheck(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit)
{
	if (OtherActor->ActorHasTag("Player"))
	{
		bCollisionWithPlayer = true;
	}
}

void AAutoPickupItem::GotoPlayer(float DeltaTime)
{
	if (bCollisionWithPlayer)
	{
		FVector PlayerLocation = PlayerPawn->GetActorLocation();
		FVector NewLocation = FMath::VInterpTo(GetActorLocation(), PlayerLocation, DeltaTime, PickUpSpeed);

		SetActorLocation(NewLocation);
	}
}

void AAutoPickupItem::PickUpEnd()
{
	if (bCollisionWithPlayer)
	{
		FVector PlayerLocation = PlayerPawn->GetActorLocation();
		if ((GetActorLocation() - PlayerLocation).Size() <= PickUpDistance)
		{
			// TODO: ���⼭ �÷��̾ ���� ���� �� �ı�
			Destroy();
		}
	}
}

void AAutoPickupItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// �浹 ���� üũ �� ����
	GotoPlayer(DeltaTime);

	// ������ ����
	PickUpEnd();
}
