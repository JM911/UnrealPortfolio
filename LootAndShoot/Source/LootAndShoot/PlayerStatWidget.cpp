// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerStatWidget.h"

#include "PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"

#include "Components/Button.h"

//bool UPlayerStatWidget::Initialize()
//{
//	Super::Initialize();
//
//	// �÷��̾� �� �ʱ�ȭ
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
	/** ���α׷� ���� �� �ݵ�� ȣ���ؾ���! => �÷��̾� ĳ���� BeginPlay�� ���� **/

	// �÷��̾� �� �ʱ�ȭ => �׳� ���ڷ� ������?
	PlayerPawn = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));

	// �ִ� ���� �ʱ�ȭ => ���ڷ� ���� ���� PlayerPawn���� �����ñ�?
	SetMaxLevel(Level);

	// ��ư ���ε�
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
	// �ʱ�ȭ
	TargetArray.Empty();

	// ������Ʈ
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
