// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ThirdPlayerCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UAnimMontage;
class AWeapon;

UCLASS()
class SHOOTANDLOOT_API AThirdPlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AThirdPlayerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// 조작
	void MoveForward(float Value);
	void MoveRight(float Value);

	// 조준(Ironsight) 관련
	void IronSightPressed();
	void IronSightReleased();

	void IronSightCameraZoom(float DeltaTime);

	// 공격 관련
	void FireWeapon();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


private:
	/** 카메라 위치 고정용 스프링 암 **/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** 플레이어 카메라 관련 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* PlayerCamera;

	// 조준 애니메이션 관련
	bool bIronSight = false;
	float CameraDefaultFOV = 0.f;
	float CameraZoomedFOV = 35.f;
	float CameraCurrentFOV = 0.f;
	float ZoomInterpSpeed = 20.f;

	// 애니메이션용 이동 방향
	float UpDownValue = 0.f;
	float LeftRightValue = 0.f;

	/** 공격 애니메이션 몽타주 **/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	UAnimMontage* FireMontage;

	// 무기 관련
	// (블프용) 기본 무기 클래스 설정 변수
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AWeapon> DefaultWeaponClass;
	// 현재 장착 중인 무기
	AWeapon* CurWeapon;
	

public:
	FORCEINLINE float GetUpDownValue() const { return UpDownValue; }
	FORCEINLINE float GetLeftRightValue() const { return LeftRightValue; }
	FORCEINLINE bool GetIronSight() const { return bIronSight; }
};
