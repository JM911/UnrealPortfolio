// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

class UCameraComponent;
class AWeapon;
class AProjectile;

UCLASS()
class SHOOTANDLOOT_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// ����
	void MoveForward(float Value);
	void MoveRight(float Value);

	// ���� ����
	void SetCrosshairLocation(float offsetX, float offsetY);

	void FireButtonPressed();
	void FireButtonReleased();
	void StartFireTimer();
	void AutoFireReset();

	void FireWeapon();
	bool TraceUnderCrosshair(FHitResult& OutHitResult, FVector& OutHitLocation);

	// ����
	void AimButtonPressed();
	void AimButtonReleased();
	void AimingCameraZoom(float DeltaTime);

	// ������
	void Reload();
	void ReloadEnd();
	void TempRefreshUI();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

		
private:
	/* Components */
	// �÷��̾� 1��Ī ī�޶�
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* PlayerCamera;

	/* Attack */
	// (������) �⺻ ���� Ŭ���� ���� ����
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AWeapon> DefaultWeaponClass;

	// ���� ���� ���� ����
	AWeapon* CurWeapon;

	// Ŭ�� Ȧ�� �� ���� ��� ����
	bool bIsAttacking = false;
	bool bFireEnable = true;
	FTimerHandle AutoFireTimer;

	// ������ ��ġ
	FVector2D CrosshairLocation = FVector2D(0.f, 0.f);

	// ����ü Ŭ���� ���� ����
	//UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true"))
	//TSubclassOf<AProjectile> CurrentProjectileClass;

	// ���� �ִϸ��̼� ����
	bool bAiming = false;
	float CameraDefaultFOV = 0.f;
	float CameraZoomedFOV = 35.f;
	float CameraCurrentFOV = 0.f;
	float ZoomInterpSpeed = 20.f;

	// ������ ����
	FTimerHandle ReloadTimer;
	bool bIsReloading = false;

public:
	FORCEINLINE FVector2D GetCrosshairLocation() const { return CrosshairLocation; }

};
