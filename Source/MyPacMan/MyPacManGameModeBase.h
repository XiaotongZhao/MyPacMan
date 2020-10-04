// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MyPacManGameModeBase.generated.h"

/**
 * 
 */
enum class EGameState : short
{
	EMenu,
	EPlaying,
	Epause,
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

private:
	EGameState currentState;
};

FORCEINLINE EGameState AMyPacManGameModeBase::GetCurrentState() const
{
	return currentState;
}