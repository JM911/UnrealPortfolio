// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "AutoPickupItem.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class EAPItemType : uint8
{
	HP		UMETA(DisplayName = "Hp"),
	MANA	UMETA(DisplayName = "Mana"),
	EXP		UMETA(DisplayName = "Exp"),

	END		UMETA(DisplayName = "End")
};

UCLASS()
class LOOTANDSHOOT_API AAutoPickupItem : public AItem
{
	GENERATED_BODY()

public:
	AAutoPickupItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// 충돌 함수
	UFUNCTION()
	void CollisionCheck(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit);
	
	// 습득 모션
	void GotoPlayer(float DeltaTime);

	// 습득 판정
	void PickUpEnd();

	// 습득 시 호출 함수
	void PickUpCallback();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	// 플레이어 폰
	class APlayerCharacter* PlayerPawn;

	// 플레이어 충돌 여부
	bool bCollisionWithPlayer = false;

	// 아이템 습득 속도
	float PickUpSpeed = 5.f;

	// 습득 판정 거리
	float PickUpDistance = 30.f;

	// 타입 (블프 수정)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Stat, meta = (AllowPrivateAccess = "true"))
	EAPItemType APItemType;

	// 체력 or 마나 or 경험치 회복량 (3가지 겸용) + 블프 수정
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Stat, meta = (AllowPrivateAccess = "true"))
	float Amount = 0.f;
	
};
