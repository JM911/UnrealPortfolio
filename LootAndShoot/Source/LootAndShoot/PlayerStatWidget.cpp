// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerStatWidget.h"

//#include "PlayerCharacter.h"
//#include "Kismet/GameplayStatics.h"

//bool UPlayerStatWidget::Initialize()
//{
//	Super::Initialize();
//
//	// 플레이어 폰 초기화
//	PlayerPawn = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
//
//	UPlayerStatComponent* PlayerStatComp = PlayerPawn->GetStatComponent();
//	if (PlayerStatComp)
//	{
//		MaxLevel = PlayerStatComp->GetMaxLevel();
//
//		SetHpChanged(PlayerStatComp->GetStatLevel(EPlayerStatType::MAX_HP));
//	}
//
//}

void UPlayerStatWidget::MyInit(int8 Level)
{
	// 최대 레벨 초기화
	SetMaxLevel(Level);
}

void UPlayerStatWidget::SetStatChanged(EPlayerStatType Type, int8 Level)
{
	switch (Type)
	{
	case EPlayerStatType::MAX_HP:
		ApplyStatChanged(HpArray, Level);
		break;
	case EPlayerStatType::MANA_MAGAZINE:
		ApplyStatChanged(ManaMagazineArray, Level);
		break;
	case EPlayerStatType::MANA_TOTAL:
		ApplyStatChanged(TotalManaArray, Level);
		break;
	case EPlayerStatType::ATTACK:
		ApplyStatChanged(AttackArray, Level);
		break;
	case EPlayerStatType::FIRE_INTERVAL:
		ApplyStatChanged(FireSpeedArray, Level);
		break;
	case EPlayerStatType::MOVE_SPEED:
		ApplyStatChanged(MoveSpeedArray, Level);
		break;
	default:
		break;
	}

	bChanged = true;
	ChangedType = Type;
}

void UPlayerStatWidget::ApplyStatChanged(TArray<bool>& TargetArray, int8 Level)
{
	// 초기화
	TargetArray.Empty();

	// 업데이트
	for (int i = 1; i <= MaxLevel; i++)
	{
		if (i <= Level)
		{
			TargetArray.Add(true);
		}
		else
		{
			TargetArray.Add(false);
		}
	}
}
