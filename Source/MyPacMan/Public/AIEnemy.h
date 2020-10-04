// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Enemy.h"
#include "AIEnemy.generated.h"

/**
 * 
 */
UCLASS()
class MYPACMAN_API AAIEnemy : public AAIController
{
	GENERATED_BODY()
public:
	void OnPossess(class APawn* InPawn) override;
	virtual void OnMoveCompleted(FAIRequestID RequestId, const FPathFollowingResult& Result);
	void SearchNewPoint();

	void GoHome();
	void ReArm();
	void StopMove();
private:
	class AEnemy* Bot;
	FVector HomeLocation;
	FTimerHandle DeadTime;
};
