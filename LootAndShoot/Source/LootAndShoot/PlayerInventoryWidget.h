// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PotionItem.h"
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerInventoryWidget.generated.h"

/**
 * 
 */
UCLASS()
class LOOTANDSHOOT_API UPlayerInventoryWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	// 플레이어 레퍼런스
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Pawn, meta = (AllowPrivateAccess = "true"))
	//class APlayerCharacter* PlayerPawn;


public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Inventory, meta = (AllowPrivateAccess = "true"))
	bool bChanged = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Inventory, meta = (AllowPrivateAccess = "true"))
	TArray<UTexture2D*> InvenTextureArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Inventory, meta = (AllowPrivateAccess = "true"))
	TArray<int32> InvenQuantityArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Inventory, meta = (AllowPrivateAccess = "true"))
	TArray<FItemStat> InvenItemStatArray;


public:
	void MyInit();

	
};
