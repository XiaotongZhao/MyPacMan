// Copyright Epic Games, Inc. All Rights Reserved.

#include "MyPacManGameModeBase.h"
#include "EngineUtils.h"

void AMyPacManGameModeBase::SetCurrentState(EGameState value)
{
	currentState = value;
	switch (value)
	{
	case EGameState::EPlaying:
		for (auto Iter(Enemys.CreateIterator()); Iter; Iter++)
			(*Iter)->SetMove(true);
		break;
	case EGameState::EPause:
		for (auto Iter(Enemys.CreateIterator()); Iter; Iter++)
			(*Iter)->SetMove(false);
		break;
	case EGameState::EWin:
		for (auto Iter(Enemys.CreateIterator()); Iter; Iter++)
			(*Iter)->Destroyed();
		break;
	case EGameState::EGameOver:
		for (auto Iter(Enemys.CreateIterator()); Iter; Iter++)
			(*Iter)->Destroyed();
		break;
	default:
		break;
	}
}

void AMyPacManGameModeBase::SetEnemyVulnerable()
{
	for (auto Iter(Enemys.CreateIterator()); Iter; Iter++)
		(*Iter)->SetVulnerable();
}

void AMyPacManGameModeBase::BeginPlay()
{
	SetCurrentState(EGameState::EMenu);
	for (TActorIterator<AEnemy> enemyItr(GetWorld()); enemyItr; ++enemyItr)
	{
		AEnemy* enemy = Cast<AEnemy>(*enemyItr);
		if (enemy)
			Enemys.Add(enemy);
	}
}
