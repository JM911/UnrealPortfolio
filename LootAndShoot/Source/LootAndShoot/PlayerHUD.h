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

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* HpBarText;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* TotalManaBarText;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ManaMagazineBarText;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ExpBarText;

	UPROPERTY(meta = (BindWidget))
	class UProgressBar* HpBar;

	UPROPERTY(meta = (BindWidget))
	class UProgressBar* TotalMpBar;

	UPROPERTY(meta = (BindWidget))
	class UProgressBar* MpMagazineBar;

	UPROPERTY(meta = (BindWidget))
	class UProgressBar* ExpBar;

public:
	FORCEINLINE UTextBlock* GetAmmoText() const { return AmmoText; }
	FORCEINLINE UTextBlock* GetHpBarText() const { return HpBarText; }
	FORCEINLINE UTextBlock* GetTotalManaBarText() const { return TotalManaBarText; }
	FORCEINLINE UTextBlock* GetManaMagazineBarText() const { return ManaMagazineBarText; }
	FORCEINLINE UTextBlock* GetExpBarText() const { return ExpBarText; }


	FORCEINLINE UProgressBar* GetHpBar() const { return HpBar; }
	FORCEINLINE UProgressBar* GetTotalMpBar() const { return TotalMpBar; }
	FORCEINLINE UProgressBar* GetMpMagazineBar() const { return MpMagazineBar; }
	FORCEINLINE UProgressBar* GetExpBar() const { return ExpBar; }
	
};
