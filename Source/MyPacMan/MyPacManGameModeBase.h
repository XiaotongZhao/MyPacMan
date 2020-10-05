// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Public/Enemy.h"
#include "MyPacManGameModeBase.generated.h"

/**
 *
 */
enum class EGameState : short
{
	EMenu,
	EPlaying,
	EPause,
	EWin,
	EGameOver
};

UCLASS()
class MYPACMAN_API AMyPacManGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:

	virtual void BeginPlay() override;
	EGameState GetCurrentState() const;
	void SetCurrentState(EGameState value);
	void SetEnemyVulnerable();

private:
	EGameState currentState;
	TArray<class AEnemy*> Enemys;
};

FORCEINLINE EGameState AMyPacManGameModeBase::GetCurrentState() const
{
	return currentState;
}