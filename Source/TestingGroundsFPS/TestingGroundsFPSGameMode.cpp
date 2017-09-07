// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "TestingGroundsFPS.h"
#include "TestingGroundsFPSGameMode.h"
#include "TestingGroundsFPSHUD.h"
#include "TestingGroundsFPSCharacter.h"

ATestingGroundsFPSGameMode::ATestingGroundsFPSGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = ATestingGroundsFPSHUD::StaticClass();
}
