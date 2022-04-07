// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerStatComponent.generated.h"

UENUM(BlueprintType)
enum class EPlayerStatType : uint8
{
	// TODO: ���� �ð��� �߰� ���
	MAX_HP			UMETA(DisplayName = "Max Hp"),
	MANA_MAGAZINE	UMETA(DisplayName = "Mana Magazine"),
	MANA_TOTAL		UMETA(DisplayName = "Total Mana"),

	ATTACK			UMETA(DisplayName = "Attack"),
	FIRE_INTERVAL	UMETA(DisplayName = "Fire Interval"),
	MOVE_SPEED		UMETA(DisplayName = "Move Speed"),

	END		UMETA(DisplayName = "End")
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LOOTANDSHOOT_API UPlayerStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPlayerStatComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	virtual void InitializeComponent() override;

	void InitStat();

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
private:

	// ���̽� ��	(���� �� �� ���� ������ �����)
	TMap<EPlayerStatType, float> StatBase;

	// ���� ����
	TMap<EPlayerStatType, float> CurrentStat;

	// ���� �� �� ���� ����
	TMap<EPlayerStatType, float> StatLevelUpRate;

	// ��Ÿ ���� �� ����
	int8 LevelUpPoint = 0;
	int8 MaxLevel = 6;
	TMap<EPlayerStatType, int8> StatLevelMap;

public:
	void LevelUpStat(EPlayerStatType Type);

	float GetCurrentStat(EPlayerStatType Type);
	int8 GetStatLevel(EPlayerStatType Type);

	FORCEINLINE int8 GetLevelUpPoint() const { return LevelUpPoint; }

	// ���� ��
	void PlayerLevelUp(int8 Point) { LevelUpPoint += Point; }
};
