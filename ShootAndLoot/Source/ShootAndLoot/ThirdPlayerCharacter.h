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

	// ����
	void MoveForward(float Value);
	void MoveRight(float Value);

	// ����(Ironsight) ����
	void IronSightPressed();
	void IronSightReleased();

	void IronSightCameraZoom(float DeltaTime);

	// ���� ����
	void FireWeapon();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


private:
	/** ī�޶� ��ġ ������ ������ �� **/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** �÷��̾� ī�޶� ���� */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* PlayerCamera;

	// ���� �ִϸ��̼� ����
	bool bIronSight = false;
	float CameraDefaultFOV = 0.f;
	float CameraZoomedFOV = 35.f;
	float CameraCurrentFOV = 0.f;
	float ZoomInterpSpeed = 20.f;

	// �ִϸ��̼ǿ� �̵� ����
	float UpDownValue = 0.f;
	float LeftRightValue = 0.f;

	/** ���� �ִϸ��̼� ��Ÿ�� **/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	UAnimMontage* FireMontage;

	// ���� ����
	// (������) �⺻ ���� Ŭ���� ���� ����
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AWeapon> DefaultWeaponClass;
	// ���� ���� ���� ����
	AWeapon* CurWeapon;
	

public:
	FORCEINLINE float GetUpDownValue() const { return UpDownValue; }
	FORCEINLINE float GetLeftRightValue() const { return LeftRightValue; }
	FORCEINLINE bool GetIronSight() const { return bIronSight; }
};
