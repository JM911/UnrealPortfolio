// Fill out your copyright notice in the Description page of Project Settings.


#include "MyHUD.h"
#include "Engine/Texture2D.h"
#include "Engine/Canvas.h"

// TODO: �÷��̾� �����ӿ� ���� ���������� ����� ���

AMyHUD::AMyHUD()
{
	// �ؽ��� �ε�
	static ConstructorHelpers::FObjectFinder<UTexture2D> CHTex(TEXT("Texture2D'/Game/_ShootAndLoot/Asset/Texture/Crosshair/Cross_Q_9.Cross_Q_9'"));
	if (CHTex.Succeeded())
	{
		CrosshairTex = CHTex.Object;
	}
}

void AMyHUD::DrawHUD()
{
	Super::DrawHUD();

	FVector2D Center;
	Canvas->GetCenter(Center.X, Center.Y);
	const FVector2D DrawPos(Center.X - CrosshairTex->GetSizeX() * 0.5f, Center.Y - CrosshairTex->GetSizeY() * 0.5f);

	DrawTextureSimple(CrosshairTex, DrawPos.X, DrawPos.Y);

	// Debug
	//UE_LOG(LogTemp, Warning, TEXT("Draw HUD"));
}
