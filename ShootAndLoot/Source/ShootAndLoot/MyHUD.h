// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MyHUD.generated.h"

/**
 * 
 */

class UTexture2D;

UCLASS()
class SHOOTANDLOOT_API AMyHUD : public AHUD
{
	GENERATED_BODY()
	
public:
	AMyHUD();

	virtual void DrawHUD() override;

private:
	UTexture2D* CrosshairTex;
};
