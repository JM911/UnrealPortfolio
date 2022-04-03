// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

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
	// �ϴ� Turn, Lookup�� �⺻�Լ� ��� => ���߿� ���� �ʿ��ϸ� �߰�
	
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
	float FireInterval = 0.2f;


	// ���� ���� ����
	int32 CurrentBullet = 0;
	int32 MagazineBullet = 10;
	int32 TotalBullet = 500;

	bool bReloading = false;
	float ReloadTime = 0.5f;
	FTimerHandle ReloadTimer;
	// TODO: ����ü ���� �ø��� �迭�� ����


};
