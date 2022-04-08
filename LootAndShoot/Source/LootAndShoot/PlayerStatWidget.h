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
		
public:
	UPlayerStatWidget();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stat, meta = (AllowPrivateAccess = "true"))
	TArray<bool> HpArray;

	
};
