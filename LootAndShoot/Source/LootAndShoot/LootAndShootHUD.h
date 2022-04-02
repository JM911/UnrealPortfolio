// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once 

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "LootAndShootHUD.generated.h"

UCLASS()
class ALootAndShootHUD : public AHUD
{
	GENERATED_BODY()

public:
	ALootAndShootHUD();

	/** Primary draw call for the HUD */
	virtual void DrawHUD() override;

private:
	/** Crosshair asset pointer */
	class UTexture2D* CrosshairTex;

};

