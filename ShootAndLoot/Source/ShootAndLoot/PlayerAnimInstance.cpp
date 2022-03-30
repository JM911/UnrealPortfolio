// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerAnimInstance.h"
#include "ThirdPlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void UPlayerAnimInstance::UpdateAnimationProperties(float DeltaTime)
{
	if (PlayerCharacter == nullptr)
	{
		PlayerCharacter = Cast<AThirdPlayerCharacter>(TryGetPawnOwner());
	}

	if (PlayerCharacter)
	{
		// �ӷ�
		FVector Velocity{ PlayerCharacter->GetVelocity() };
		Velocity.Z = 0.f;
		Speed = Velocity.Size();

		// ���� (-180 ~ 180)
		//Direction = CalculateDirection(PlayerCharacter->GetVelocity(), PlayerCharacter->GetActorRotation());

		// ���� (-180 ~ 180)
		FRotator AimRotation = PlayerCharacter->GetBaseAimRotation();
		FRotator MovementRotation = UKismetMathLibrary::MakeRotFromX(
			PlayerCharacter->GetVelocity());
		MovementOffsetYaw = UKismetMathLibrary::NormalizedDeltaRotator(
			MovementRotation,
			AimRotation).Yaw;

		// �̵� ���� (-1 ~ 1)
		VerticalDir = PlayerCharacter->GetUpDownValue();
		HorizontalDir = PlayerCharacter->GetLeftRightValue();

		// Pitch(-180 ~ 180)
		AimRotationPitch = PlayerCharacter->GetBaseAimRotation().Pitch;

		// ���� ����
		bIsInAir = PlayerCharacter->GetCharacterMovement()->IsFalling();

		// ���� ����
		bIronSight = PlayerCharacter->GetIronSight();
	}
}

void UPlayerAnimInstance::NativeInitializeAnimation()
{
	PlayerCharacter = Cast<AThirdPlayerCharacter>(TryGetPawnOwner());
}
