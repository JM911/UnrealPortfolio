// Copyright Epic Games, Inc. All Rights Reserved.


#include "ShootAndLootGameModeBase.h"
#include "PlayerCharacter.h"


AShootAndLootGameModeBase::AShootAndLootGameModeBase()
{
	static ConstructorHelpers::FClassFinder<APlayerCharacter> BP_Char(TEXT("Blueprint'/Game/_ShootAndLoot/Character/Player/BP_PlayerCharacter.BP_PlayerCharacter_C'"));
	if (BP_Char.Succeeded())
	{
		DefaultPawnClass = BP_Char.Class;
	}
}
