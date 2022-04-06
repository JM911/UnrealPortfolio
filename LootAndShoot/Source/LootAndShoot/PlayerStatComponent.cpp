// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerStatComponent.h"

// Sets default values for this component's properties
UPlayerStatComponent::UPlayerStatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	bWantsInitializeComponent = true;

	// ...
}


// Called when the game starts
void UPlayerStatComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
}

void UPlayerStatComponent::InitializeComponent()
{
	Super::InitializeComponent();

	InitStat();
}

void UPlayerStatComponent::InitStat()
{
	// ���̽� �� ����
	StatBase.Add(MakeTuple(EPlayerStatType::MAX_HP, 100.f));
	StatBase.Add(MakeTuple(EPlayerStatType::MANA_MAGAZINE, 5.f));// ���� ���ӿ��� int�� ����ȯ
	StatBase.Add(MakeTuple(EPlayerStatType::MANA_TOTAL, 100.f)); // ���� ���ӿ��� int�� ����ȯ
	StatBase.Add(MakeTuple(EPlayerStatType::ATTACK, 10.f));
	StatBase.Add(MakeTuple(EPlayerStatType::FIRE_INTERVAL, 1.f));
	StatBase.Add(MakeTuple(EPlayerStatType::MOVE_SPEED, 1.f));

	// ���� �ʱ� ���� (���̽� ���� �Ȱ���)
	for (auto StatTuple : StatBase)
	{
		CurrentStat.Add(StatTuple);
	}

	// ���� �� ���� ���� ����
	StatLevelUpRate.Add(MakeTuple(EPlayerStatType::MAX_HP, 0.2f));
	StatLevelUpRate.Add(MakeTuple(EPlayerStatType::MANA_MAGAZINE, 0.2f));
	StatLevelUpRate.Add(MakeTuple(EPlayerStatType::MANA_TOTAL, 1.f));
	StatLevelUpRate.Add(MakeTuple(EPlayerStatType::ATTACK, 0.5f));
	StatLevelUpRate.Add(MakeTuple(EPlayerStatType::FIRE_INTERVAL, -0.1f));
	StatLevelUpRate.Add(MakeTuple(EPlayerStatType::MOVE_SPEED, 0.1f));

	// ���� ���� ��Ȳ ����
	StatLevelMap.Add(MakeTuple(EPlayerStatType::MAX_HP, 1));
	StatLevelMap.Add(MakeTuple(EPlayerStatType::MANA_MAGAZINE, 1));
	StatLevelMap.Add(MakeTuple(EPlayerStatType::MANA_TOTAL, 1));
	StatLevelMap.Add(MakeTuple(EPlayerStatType::ATTACK, 1));
	StatLevelMap.Add(MakeTuple(EPlayerStatType::FIRE_INTERVAL, 1));
	StatLevelMap.Add(MakeTuple(EPlayerStatType::MOVE_SPEED, 1));
}


// Called every frame
void UPlayerStatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UPlayerStatComponent::LevelUpStat(EPlayerStatType Type)
{
	// ���� �� ����Ʈ üũ
	if (LevelUpPoint <= 0)
		return;

	// ���ϴ� ���� ���� üũ
	int curStatLevel = *StatLevelMap.Find(Type);
	if (curStatLevel >= MaxLevel)
		return;

	// ���� ��
	CurrentStat[Type] += StatBase[Type] * StatLevelUpRate[Type];

	// ���� ���� ����
	StatLevelMap[Type]++;

	// ����� ���� �� ����Ʈ ����
	LevelUpPoint--;
}

float UPlayerStatComponent::GetCurrentStat(EPlayerStatType Type)
{
	if (CurrentStat.Find(Type))
		return CurrentStat[Type];

	return 0.f;
}
