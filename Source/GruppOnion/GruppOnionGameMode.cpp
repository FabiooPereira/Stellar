// Copyright Epic Games, Inc. All Rights Reserved.

#include "GruppOnionGameMode.h"
#include "GruppOnionCharacter.h"
#include "UObject/ConstructorHelpers.h"

AGruppOnionGameMode::AGruppOnionGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
