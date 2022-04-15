// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PotionItem.h"

#include "PlayerStatComponent.h"

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
	void Turn(float Value);
	void LookUp(float Value);
	
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

	// 플레이어 HUD 업데이트
	void UpdatePlayerHUD();

	// 아이템 관련
	UFUNCTION()
	void TakeItem();

	bool bInventoryToggle = false;
	void InventoryWidgetToggle();

	void InventoryWidgetUpdate();

	// 스탯 관련 => 레벨업 부분 리팩토링 가능? (일단은 바인딩때문에 스탯 별로 따로 작성)
	void StatUpdate(EPlayerStatType Type);
	void AllStatUpdate();
	
	void LevelUpMaxHp();
	void LevelUpManaMagazine();
	void LevelUpManaToTal();
	void LevelUpAttack();
	void LevelUpFireInterval();
	void LevelUpMoveSpeed();

	void StatWidgetUpdate(EPlayerStatType Type);
	void StatWidgetLevelUpPointUpdate();

	bool bStatWidgetToggle = false;
	void StatWidgetToggle();

	// 버프 관련
	bool bInvincible = false;
	FTimerHandle InvincibleTimer;

	void SetPlayerInvincible(float InvincibleTime = 10.f);	// 물약 정보에서 가져올까?

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



	// 장전 관련 변수
	bool bReloading = false;
	float ReloadTime = 1.f;
	FTimerHandle ReloadTimer;

	float CurrentReloadTime = 0.f;	// 장전 위젯 전달용


	// 아이템 인벤토리 관련
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Item, meta = (AllowPrivateAccess = "true"))
	TMap<EPotionType, TPair<FItemStat, int32>> Inventory;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Item, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UPlayerInventoryWidget> InvenWidgetClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Item, meta = (AllowPrivateAccess = "true"))
	class UPlayerInventoryWidget* InvenWidget;


	// 스탯 
	class UPlayerStatComponent* StatComponent;

	float CurrentHp = 0.f;
	float MaxHp = 0.f;

	int32 CurrentMana = 0;
	int32 MagazineMana = 10;
	int32 CurrentTotalMana = 0;
	int32 MaxTotalMana = 500;

	float Attack = 0.f;
	float FireInterval = 0.2f;

	float BaseSpeed = 600.f;
	float MoveSpeedRate = 0.f;
	
	float CurrentExp = 0.f;
	float NextLevelExp = 100.f;


	// 스탯 위젯
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Item, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UPlayerStatWidget> StatWidgetClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Item, meta = (AllowPrivateAccess = "true"))
	class UPlayerStatWidget* StatWidget;

	// 장전 위젯
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Item, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UUserWidget> RelaodWidgetClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Item, meta = (AllowPrivateAccess = "true"))
	class UUserWidget* RelaodWidget;


public:
	/** 즉시 습득 아이템 관련 함수들 */
	// 체력 회복
	void AddHp(float Amount);

	// 마나 회복
	void AddMana(float Amount);

	// 경험치 증가 및 레벨 업 체크
	void AddExp(float Amount);


	/** 스탯 관련 */
	UPlayerStatComponent* GetStatComponent() const { return StatComponent; }

	void LevelUpStat(EPlayerStatType Type);


	/** 물약 사용 관련 */
	UFUNCTION(BlueprintCallable)
	void DrinkPotion(EPotionType Type);
	
	// 디버깅용 헬퍼 함수(Enum을 FString으로)
	FString GetEStateAsString(EPotionType Type);

	/* 데미지 */
	void TakeDamage(float Amount);

	UFUNCTION(BlueprintCallable)
	float GetCurrentReloadPercent() const { return CurrentReloadTime / ReloadTime; }
};
