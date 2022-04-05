// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PotionItem.h"

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

UCLASS()
class LOOTANDSHOOT_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// 움직임
	void MoveForward(float Value);
	void MoveRight(float Value);
	// 일단 Turn, Lookup은 기본함수 사용 => 나중에 감도 필요하면 추가
	
	/** 공격 관련 */
	void FireProjectile();
	
	// 연사
	void FireButtonPressed();
	void FireButtonReleased();
	void FireNextReady();
	void FireFinal();

	// 장전
	void Reload();
	void ReloadEnd();

	// 탄약 관련 UI 업데이트
	void UpdatePlayerHUD();

	// 아이템 관련
	UFUNCTION()
	void TakeItem();

	bool bInventoryToggle = false;
	void InventoryWidgetToggle();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


private:

	// 카메라
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FirstPersonCamera;

	// 손 메쉬
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	class USkeletalMeshComponent* HandsMesh;

	// 총 메쉬
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	class USkeletalMeshComponent* GunMesh;

	// 투사체 발사 위치 저장용
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	class USceneComponent* MuzzleLocation;

	// 투사체 클래스(블프 설정)
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Projectile, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class AProjectile> ProjectileClass;

	
	// 투사체 연사 기능용 변수
	bool bFireButtonPressed = false;
	bool bFireReady = true;
	FTimerHandle FireTimer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	float FireInterval = 0.2f;


	// 장전 관련 변수
	int32 CurrentMana = 0;
	int32 MagazineMana = 10;
	int32 TotalMana = 500;

	bool bReloading = false;
	float ReloadTime = 0.5f;
	FTimerHandle ReloadTimer;
	// TODO: 투사체 종류 늘리면 배열로 설정


	// 아이템 인벤토리 관련
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Item, meta = (AllowPrivateAccess = "true"))
	TMap<EPotionType, TPair<FItemStat, int32>> Inventory;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Item, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UPlayerInventoryWidget> InvenWidgetClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Item, meta = (AllowPrivateAccess = "true"))
	class UPlayerInventoryWidget* InvenWidget;

};
