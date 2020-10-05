// Copyright Epic Games, Inc. All Rights Reserved.

#include "MyPacManGameModeBase.h"
#include "EngineUtils.h"

void AMyPacManGameModeBase::SetCurrentState(EGameState value)
{
	currentState = value;
}

void AMyPacManGameModeBase::SetEnemyVulnerable()
{
	for (auto Iter(Enemys.CreateIterator()); Iter; Iter++)
	{
		(*Iter)->SetVulnerable();
	}
}

void AMyPacManGameModeBase::BeginPlay()
{
	SetCurrentState(EGameState::EPlaying);
	for (TActorIterator<AEnemy> enemyItr(GetWorld()); enemyItr; ++enemyItr)
	{
		AEnemy* enemy = Cast<AEnemy>(*enemyItr);
		if (enemy)
			Enemys.Add(enemy);
	}
}
