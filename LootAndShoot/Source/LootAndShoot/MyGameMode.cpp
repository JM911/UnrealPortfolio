// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameMode.h"

#include "PlayerHUD.h"

AMyGameMode::AMyGameMode()	:
	Super()
{

}

void AMyGameMode::BeginPlay()
{
	Super::BeginPlay();

	// HUD ºäÆ÷Æ® Ãß°¡
	if (PlayerHUDClass)
	{
		CurrentWidget = CreateWidget(GetWorld(), PlayerHUDClass);

		if (CurrentWidget)
		{
			CurrentWidget->AddToViewport();
		}
	}
}
