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

	// �浹 �Լ�
	UFUNCTION()
	void CollisionCheck(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit);
	
	// ���� ���
	void GotoPlayer(float DeltaTime);

	// ���� ����
	void PickUpEnd();

	// ���� �� ȣ�� �Լ�
	void PickUpCallback();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	// �÷��̾� ��
	class APlayerCharacter* PlayerPawn;

	// �÷��̾� �浹 ����
	bool bCollisionWithPlayer = false;

	// ������ ���� �ӵ�
	float PickUpSpeed = 5.f;

	// ���� ���� �Ÿ�
	float PickUpDistance = 30.f;

	// Ÿ�� (���� ����)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Stat, meta = (AllowPrivateAccess = "true"))
	EAPItemType APItemType;

	// ü�� or ���� or ����ġ ȸ���� (3���� ���) + ���� ����
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Stat, meta = (AllowPrivateAccess = "true"))
	float Amount = 0.f;
	
};
