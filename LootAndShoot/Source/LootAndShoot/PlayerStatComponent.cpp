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
	// 베이스 값 설정
	StatBase.Add(MakeTuple(EPlayerStatType::MAX_HP, 100.f));
	StatBase.Add(MakeTuple(EPlayerStatType::MANA_MAGAZINE, 5.f));// 실제 게임에선 int로 형변환
	StatBase.Add(MakeTuple(EPlayerStatType::MANA_TOTAL, 100.f)); // 실제 게임에선 int로 형변환
	StatBase.Add(MakeTuple(EPlayerStatType::ATTACK, 10.f));
	StatBase.Add(MakeTuple(EPlayerStatType::FIRE_INTERVAL, 1.f));
	StatBase.Add(MakeTuple(EPlayerStatType::MOVE_SPEED, 1.f));

	// 스탯 초기 설정 (베이스 값과 똑같게)
	for (auto StatTuple : StatBase)
	{
		CurrentStat.Add(StatTuple);
	}

	// 레벨 업 증가 비율 설정
	StatLevelUpRate.Add(MakeTuple(EPlayerStatType::MAX_HP, 0.2f));
	StatLevelUpRate.Add(MakeTuple(EPlayerStatType::MANA_MAGAZINE, 0.2f));
	StatLevelUpRate.Add(MakeTuple(EPlayerStatType::MANA_TOTAL, 1.f));
	StatLevelUpRate.Add(MakeTuple(EPlayerStatType::ATTACK, 0.5f));
	StatLevelUpRate.Add(MakeTuple(EPlayerStatType::FIRE_INTERVAL, -0.15f));
	StatLevelUpRate.Add(MakeTuple(EPlayerStatType::MOVE_SPEED, 0.3f));

	// 스탯 레벨 상황 저장
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
	// 레벨 업 포인트 체크
	if (LevelUpPoint <= 0)
		return;

	// 원하는 스탯 레벨 체크
	int curStatLevel = *StatLevelMap.Find(Type);
	if (curStatLevel >= MaxLevel)
		return;

	// 레벨 업
	CurrentStat[Type] += StatBase[Type] * StatLevelUpRate[Type];

	// 공격 간격 최소치 체크 (다른 스탯도 체크해야 하면 그냥 리팩토링)
	if (CurrentStat[EPlayerStatType::FIRE_INTERVAL] < 0.1f)
		CurrentStat[EPlayerStatType::FIRE_INTERVAL] = 0.1f;

	// 스탯 레벨 증가
	StatLevelMap[Type]++;

	// 사용한 레벨 업 포인트 차감
	LevelUpPoint--;
}

float UPlayerStatComponent::GetCurrentStat(EPlayerStatType Type)
{
	if (CurrentStat.Find(Type))
		return CurrentStat[Type];

	return 0.f;
}

int8 UPlayerStatComponent::GetStatLevel(EPlayerStatType Type)
{
	if (StatLevelMap.Find(Type))
		return StatLevelMap[Type];

	return 0;
}
