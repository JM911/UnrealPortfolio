// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

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
	class APlayerCharacter* PlayerPawn;

	int8 MaxLevel = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Stat, meta = (AllowPrivateAccess = "true"))
	bool bChanged = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stat, meta = (AllowPrivateAccess = "true"))
	TArray<bool> HpArray;


public:
	void SetMaxLevel(int8 Level) { MaxLevel = Level; }

	void SetHpChanged(int8 Level);

};
