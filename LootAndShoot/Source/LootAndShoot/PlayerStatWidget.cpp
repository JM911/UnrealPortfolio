// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerStatWidget.h"

#include "PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerStatComponent.h"

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

void UPlayerStatWidget::SetHpChanged(int8 Level)
{
	// �ʱ�ȭ
	HpArray.Empty();

	// ������Ʈ
	for (int i = 1; i <= MaxLevel; i++)
	{
		if (i <= Level)
		{
			HpArray.Add(true);
		}
		else
		{
			HpArray.Add(false);
		}
	}

	bChanged = true;
}
