// Copyright Epic Games, Inc. All Rights Reserved.

#include "LooterShooterGameMode.h"
#include "LooterShooterCharacter.h"
#include "UObject/ConstructorHelpers.h"

ALooterShooterGameMode::ALooterShooterGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}
