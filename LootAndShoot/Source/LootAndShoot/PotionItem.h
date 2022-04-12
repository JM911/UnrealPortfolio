// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "PotionItem.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class EPotionType : uint8
{
	NONE	UMETA(DisplayName = "None"),

	HP		UMETA(DisplayName = "Hp"),
	MANA_1	UMETA(DisplayName = "Mana_1"),

	BUFF_1	UMETA(DisplayName = "Buff_1"),


	END		UMETA(DisplayName = "End")
};

USTRUCT(Atomic, BlueprintType)
struct FItemStat
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EPotionType Type;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UTexture2D* Icon;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString Description;
};

UCLASS()
class LOOTANDSHOOT_API APotionItem : public AItem
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stat, meta = (AllowPrivateAccess = "true"))
	FItemStat ItemStat;

public:
	FORCEINLINE FItemStat GetItemStat() const { return ItemStat; }

	void Interact();


};
