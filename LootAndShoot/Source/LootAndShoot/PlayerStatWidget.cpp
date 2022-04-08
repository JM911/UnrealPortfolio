// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerStatWidget.h"

#include "PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerStatComponent.h"

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

void UPlayerStatWidget::SetHpChanged(int8 Level)
{
	// 초기화
	HpArray.Empty();

	// 업데이트
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
