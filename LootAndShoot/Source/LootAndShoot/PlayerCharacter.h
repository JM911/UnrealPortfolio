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

	// ������
	void MoveForward(float Value);
	void MoveRight(float Value);
	void Turn(float Value);
	void LookUp(float Value);
	
	/** ���� ���� */
	void FireProjectile();
	
	// ����
	void FireButtonPressed();
	void FireButtonReleased();
	void FireNextReady();
	void FireFinal();

	// ����
	void Reload();
	void ReloadEnd();

	// ź�� ���� UI ������Ʈ
	void UpdatePlayerHUD();

	// ������ ����
	UFUNCTION()
	void TakeItem();

	bool bInventoryToggle = false;
	void InventoryWidgetToggle();

	// ���� ���� => ������ �κ� �����丵 ����? (�ϴ��� ���ε������� ���� ���� ���� �ۼ�)
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

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


private:

	// ī�޶�
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FirstPersonCamera;

	// �� �޽�
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	class USkeletalMeshComponent* HandsMesh;

	// �� �޽�
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	class USkeletalMeshComponent* GunMesh;

	// ����ü �߻� ��ġ �����
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	class USceneComponent* MuzzleLocation;

	// ����ü Ŭ����(���� ����)
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Projectile, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class AProjectile> ProjectileClass;

	
	// ����ü ���� ��ɿ� ����
	bool bFireButtonPressed = false;
	bool bFireReady = true;
	FTimerHandle FireTimer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))



	// ���� ���� ����
	bool bReloading = false;
	float ReloadTime = 0.5f;
	FTimerHandle ReloadTimer;


	// ������ �κ��丮 ����
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Item, meta = (AllowPrivateAccess = "true"))
	TMap<EPotionType, TPair<FItemStat, int32>> Inventory;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Item, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UPlayerInventoryWidget> InvenWidgetClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Item, meta = (AllowPrivateAccess = "true"))
	class UPlayerInventoryWidget* InvenWidget;


	// ���� 
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


	// ���� ����
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Item, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UPlayerStatWidget> StatWidgetClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Item, meta = (AllowPrivateAccess = "true"))
	class UPlayerStatWidget* StatWidget;


public:
	/** ��� ���� ������ ���� �Լ��� */
	// ü�� ȸ��
	void AddHp(float Amount);

	// ���� ȸ��
	void AddMana(float Amount);

	// ����ġ ���� �� ���� �� üũ
	void AddExp(float Amount);


	/** ���� ���� */
	UPlayerStatComponent* GetStatComponent() const { return StatComponent; }

	void LevelUpStat(EPlayerStatType Type);


	/** ���� ��� ���� */
	UFUNCTION(BlueprintCallable)
	void DrinkPotion(EPotionType Type);
	
	// ������ ���� �Լ�(Enum�� FString����)
	FString GetEStateAsString(EPotionType Type);
};
