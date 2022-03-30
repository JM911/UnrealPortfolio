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

	// 조작
	void MoveForward(float Value);
	void MoveRight(float Value);

	// 공격 관련
	void SetCrosshairLocation(float offsetX, float offsetY);

	void FireButtonPressed();
	void FireButtonReleased();
	void StartFireTimer();
	void AutoFireReset();

	void FireWeapon();
	bool TraceUnderCrosshair(FHitResult& OutHitResult, FVector& OutHitLocation);

	// 조준
	void AimButtonPressed();
	void AimButtonReleased();
	void AimingCameraZoom(float DeltaTime);

	// 재장전
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
	// 플레이어 1인칭 카메라
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* PlayerCamera;

	/* Attack */
	// (블프용) 기본 무기 클래스 설정 변수
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AWeapon> DefaultWeaponClass;

	// 현재 장착 중인 무기
	AWeapon* CurWeapon;

	// 클릭 홀드 시 연발 사격 관련
	bool bIsAttacking = false;
	bool bFireEnable = true;
	FTimerHandle AutoFireTimer;

	// 조준점 위치
	FVector2D CrosshairLocation = FVector2D(0.f, 0.f);

	// 투사체 클래스 설정 변수
	//UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true"))
	//TSubclassOf<AProjectile> CurrentProjectileClass;

	// 조준 애니메이션 관련
	bool bAiming = false;
	float CameraDefaultFOV = 0.f;
	float CameraZoomedFOV = 35.f;
	float CameraCurrentFOV = 0.f;
	float ZoomInterpSpeed = 20.f;

	// 재장전 관련
	FTimerHandle ReloadTimer;
	bool bIsReloading = false;

public:
	FORCEINLINE FVector2D GetCrosshairLocation() const { return CrosshairLocation; }

};
