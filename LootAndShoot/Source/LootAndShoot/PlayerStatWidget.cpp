// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerStatWidget.h"

#include "PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"

#include "Components/Button.h"

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

void UPlayerStatWidget::HpButtonClicked()
{
	PlayerPawn->LevelUpStat(EPlayerStatType::MAX_HP);
}

void UPlayerStatWidget::ManaMagazineButtonClicked()
{
	PlayerPawn->LevelUpStat(EPlayerStatType::MANA_MAGAZINE);
}

void UPlayerStatWidget::TotalManaButtonClicked()
{
	PlayerPawn->LevelUpStat(EPlayerStatType::MANA_TOTAL);
}

void UPlayerStatWidget::AttackButtonClicked()
{
	PlayerPawn->LevelUpStat(EPlayerStatType::ATTACK);
}

void UPlayerStatWidget::FireSpeedButtonClicked()
{
	PlayerPawn->LevelUpStat(EPlayerStatType::FIRE_INTERVAL);
}

void UPlayerStatWidget::MoveSpeedButtonClicked()
{
	PlayerPawn->LevelUpStat(EPlayerStatType::MOVE_SPEED);
}

void UPlayerStatWidget::MyInit(int8 Level)
{
	/** 어딘가에서 반드시 호출해야함! => Level 인자를 받으니 플레이어 캐릭터 BeginPlay가 적절 **/

	// 플레이어 폰 초기화 => 그냥 인자로 받을까?
	PlayerPawn = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));

	// 최대 레벨 초기화 => 인자로 받지 말고 PlayerPawn에서 꺼내올까?
	SetMaxLevel(Level);

	// 버튼 바인딩
	HpButton->OnClicked.AddDynamic(this, &UPlayerStatWidget::HpButtonClicked);
	ManaMagazineButton->OnClicked.AddDynamic(this, &UPlayerStatWidget::ManaMagazineButtonClicked);
	TotalManaButton->OnClicked.AddDynamic(this, &UPlayerStatWidget::TotalManaButtonClicked);
	AttackButton->OnClicked.AddDynamic(this, &UPlayerStatWidget::AttackButtonClicked);
	FireSpeedButton->OnClicked.AddDynamic(this, &UPlayerStatWidget::FireSpeedButtonClicked);
	MoveSpeedButton->OnClicked.AddDynamic(this, &UPlayerStatWidget::MoveSpeedButtonClicked);
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
