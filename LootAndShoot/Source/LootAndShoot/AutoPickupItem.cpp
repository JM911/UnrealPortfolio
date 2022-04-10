// Fill out your copyright notice in the Description page of Project Settings.


#include "AutoPickupItem.h"

#include "Components/SphereComponent.h"

#include "PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/ProjectileMovementComponent.h"

AAutoPickupItem::AAutoPickupItem()
{
	// 틱 이벤트 on
	PrimaryActorTick.bCanEverTick = true;

	// 이미 가지고 있는 충돌 구체 크기 키우기
	GetSphereCollision()->InitSphereRadius(200.f);

	// 자동습득 템만 흩뿌려지도록
	GetItemMovement()->InitialSpeed = 300.f;
	GetItemMovement()->MaxSpeed = 500.f;
}

void AAutoPickupItem::BeginPlay()
{
	Super::BeginPlay();

	// 플레이어 폰 초기화
	PlayerPawn = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));

	// 충돌 함수 삽입
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
			PickUpCallback();
			Destroy();
		}
	}
}

void AAutoPickupItem::PickUpCallback()
{
	switch (APItemType)
	{
	case EAPItemType::HP:
		PlayerPawn->AddHp(Amount);
		break;
	case EAPItemType::MANA:
		PlayerPawn->AddMana(Amount);
		break;
	case EAPItemType::EXP:
		PlayerPawn->AddExp(Amount);
		break;
	default:
		break;
	}
}

void AAutoPickupItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 충돌 여부 체크 및 습득
	GotoPlayer(DeltaTime);

	// 가까우면 삭제
	PickUpEnd();
}
