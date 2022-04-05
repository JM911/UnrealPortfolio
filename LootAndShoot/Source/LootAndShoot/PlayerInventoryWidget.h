// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

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

	// �ϴ� �ۺ����� �׽�Ʈ
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Inventory, meta = (AllowPrivateAccess = "true"))
	bool bChanged = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Inventory, meta = (AllowPrivateAccess = "true"))
	TArray<UTexture2D*> InvenTextureArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Inventory, meta = (AllowPrivateAccess = "true"))
	TArray<int32> InvenQuantityArray;
	
};
