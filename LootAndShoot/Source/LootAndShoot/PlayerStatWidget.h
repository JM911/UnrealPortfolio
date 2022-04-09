// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PlayerStatComponent.h"

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerStatWidget.generated.h"

/**
 * 
 */

UCLASS()
class LOOTANDSHOOT_API UPlayerStatWidget : public UUserWidget
{
	GENERATED_BODY()

//public:
//	virtual bool Initialize() override;

protected:
	//class APlayerCharacter* PlayerPawn;

	int8 MaxLevel = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Stat, meta = (AllowPrivateAccess = "true"))
	bool bChanged = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Stat, meta = (AllowPrivateAccess = "true"))
	EPlayerStatType ChangedType;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stat, meta = (AllowPrivateAccess = "true"))
	TArray<bool> HpArray;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stat, meta = (AllowPrivateAccess = "true"))
	TArray<bool> ManaMagazineArray;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stat, meta = (AllowPrivateAccess = "true"))
	TArray<bool> TotalManaArray;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stat, meta = (AllowPrivateAccess = "true"))
	TArray<bool> AttackArray;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stat, meta = (AllowPrivateAccess = "true"))
	TArray<bool> FireSpeedArray;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stat, meta = (AllowPrivateAccess = "true"))
	TArray<bool> MoveSpeedArray;

	void SetMaxLevel(int8 Level) { MaxLevel = Level; }


	UPROPERTY(meta = (BindWidget))
	class UTextBlock* LevelUpPointText;

public:
	void MyInit(int8 Level);

	void SetStatChanged(EPlayerStatType Type, int8 Level);

	//void SetHpChanged(int8 Level);

	void ApplyStatChanged(TArray<bool>& TargetArray, int8 Level);


	FORCEINLINE UTextBlock* GetLevelUpPointText() const { return LevelUpPointText; }

};
