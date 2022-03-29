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
	bool GetBeamEndLocation(const FVector& BarrelSocketLocation, FVector& BeamEndLocation);
	void FireWeapon();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

		
private:
	/** 플레이어 1인칭 카메라 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* PlayerCamera;

	/** (블프용) 기본 무기 클래스 설정 변수 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AWeapon> DefaultWeaponClass;

	FVector2D CrosshairLocation = FVector2D(0.f, 0.f);

	/** 투사체 클래스 설정 변수 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AProjectile> CurrentProjectileClass;
	

public:
	FORCEINLINE FVector2D GetCrosshairLocation() const { return CrosshairLocation; }


};
