// Copyright Epic Games, Inc. All Rights Reserved.


#include "MyPacManGameModeBase.h"

void AMyPacManGameModeBase::SetCurrentState(EGameState value)
{
	currentState = value;
}

void AMyPacManGameModeBase::BeginPlay()
{
	SetCurrentState(EGameState::EPlaying);
}
