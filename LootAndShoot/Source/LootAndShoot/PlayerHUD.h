// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerHUD.generated.h"

/**
 * 
 */
UCLASS()
class LOOTANDSHOOT_API UPlayerHUD : public UUserWidget
{
	GENERATED_BODY()

private:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* AmmoText;

public:
	FORCEINLINE UTextBlock* GetAmmoText() const { return AmmoText; }
	
};
