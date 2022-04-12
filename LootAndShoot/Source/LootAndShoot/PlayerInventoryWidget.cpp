// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerInventoryWidget.h"

#include "PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Button.h"

void UPlayerInventoryWidget::MyInit()
{
	/** 프로그램 실행 시 반드시 호출해야함! => 플레이어 캐릭터 BeginPlay가 적절 **/

	// 플레이어 폰 초기화 => 그냥 인자로 받을까?
	//PlayerPawn = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
}
