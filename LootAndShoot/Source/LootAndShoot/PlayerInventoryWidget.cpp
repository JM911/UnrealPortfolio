// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerInventoryWidget.h"

#include "PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Button.h"

void UPlayerInventoryWidget::MyInit()
{
	/** ���α׷� ���� �� �ݵ�� ȣ���ؾ���! => �÷��̾� ĳ���� BeginPlay�� ���� **/

	// �÷��̾� �� �ʱ�ȭ => �׳� ���ڷ� ������?
	//PlayerPawn = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
}
