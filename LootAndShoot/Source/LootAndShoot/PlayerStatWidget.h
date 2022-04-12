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
	// 플레이어 레퍼런스
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Pawn, meta = (AllowPrivateAccess = "true"))
	class APlayerCharacter* PlayerPawn;

	// 스탯 업데이트 관련
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

	// 위젯 변수들
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* LevelUpPointText;

	UPROPERTY(meta = (BindWidget))
	class UButton* HpButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* ManaMagazineButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* TotalManaButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* AttackButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* FireSpeedButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* MoveSpeedButton;

	// 버튼 액션
	UFUNCTION()
	void HpButtonClicked();

	UFUNCTION()
	void ManaMagazineButtonClicked();

	UFUNCTION()
	void TotalManaButtonClicked();

	UFUNCTION()
	void AttackButtonClicked();

	UFUNCTION()
	void FireSpeedButtonClicked();

	UFUNCTION()
	void MoveSpeedButtonClicked();

public:
	void MyInit(int8 Level);

	void SetStatChanged(EPlayerStatType Type, int8 Level);

	//void SetHpChanged(int8 Level);

	void ApplyStatChanged(TArray<bool>& TargetArray, int8 Level);


	FORCEINLINE UTextBlock* GetLevelUpPointText() const { return LevelUpPointText; }

};
