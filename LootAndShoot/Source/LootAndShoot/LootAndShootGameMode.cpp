// Copyright Epic Games, Inc. All Rights Reserved.

#include "LootAndShootGameMode.h"
#include "LootAndShootHUD.h"
#include "LootAndShootCharacter.h"
#include "UObject/ConstructorHelpers.h"

ALootAndShootGameMode::ALootAndShootGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = ALootAndShootHUD::StaticClass();
}
