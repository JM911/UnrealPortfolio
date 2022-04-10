// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item.generated.h"

UCLASS()
class LOOTANDSHOOT_API AItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


private:
	// 바닥 충돌용 박스(루트)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* FloorCollision;

	// 아이템 메쉬
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* ItemMesh;

	// 충돌 구체(아이템 상호작용 범위 체크용) => 세부 조정은 블프에서
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Collision, meta = (AllowPrivateAccess = "true"))
	class USphereComponent* SphereCollision;

	// 아이템 스폰 시 움직임을 위한 컴포넌트
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	class UProjectileMovementComponent* ItemMovement;

public:
	//FORCEINLINE UBoxComponent* GetBoxCollision() const { return BoxCollision; }
	FORCEINLINE USphereComponent* GetSphereCollision() const { return SphereCollision; }
	FORCEINLINE UProjectileMovementComponent* GetItemMovement() const { return ItemMovement; }
};
