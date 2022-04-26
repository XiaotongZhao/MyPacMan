// Fill out your copyright notice in the Description page of Project Settings.


#include "PacManCharacter.h"
#include "Collectables.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EngineUtils.h"

// Sets default values
APacManCharacter::APacManCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	GetCharacterMovement()->JumpZVelocity = 400.f;
	GetCharacterMovement()->AirControl = 0.2f;
}

// Called when the game starts or when spawned
void APacManCharacter::BeginPlay()
{
	Super::BeginPlay();
	Lives = 3;
	GameMode = Cast<AMyPacManGameModeBase>(UGameplayStatics::GetGameMode(this));
	StartPoint = GetActorLocation();
	UE_LOG(LogTemp, Warning, TEXT("StartPoint: %s"), *StartPoint.ToString());
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &APacManCharacter::OnCollision);
	for (TActorIterator<ACollectables> CollectableItr(GetWorld()); CollectableItr; ++CollectableItr)
	{
		CollectablesToEat++;
	}
	UE_LOG(LogTemp, Warning, TEXT("Total Collectable is %d"), CollectablesToEat);
}

// Called every frame
void APacManCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APacManCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis("MoveX", this, &APacManCharacter::MoveXAxis);
	PlayerInputComponent->BindAxis("MoveY", this, &APacManCharacter::MoveYAxis);

	PlayerInputComponent->BindAction("NewGame", IE_Pressed, this, &APacManCharacter::NewGame);
	PlayerInputComponent->BindAction("ReStart", IE_Pressed, this, &APacManCharacter::ReStart);
	PlayerInputComponent->BindAction("Pause", IE_Pressed, this, &APacManCharacter::Pause);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &APacManCharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &APacManCharacter::StopJumping);
}

void APacManCharacter::MoveXAxis(float AxisValue)
{
	if (GameMode->GetCurrentState() == EGameState::EPlaying && Controller != nullptr && AxisValue != 0.0f)
	{
		CurrentVelocity.X = AxisValue;
		CurrentVelocity.Y = 0.f;
		CurrentRotation = Controller->GetControlRotation();
		CurrentRotation.Yaw = AxisValue > 0.0f ? 0.f : 180.f;
		AddMovementInput(CurrentVelocity);
		Controller->SetControlRotation(CurrentRotation);
	}
}

void APacManCharacter::MoveYAxis(float AxisValue)
{
	if (GameMode->GetCurrentState() == EGameState::EPlaying && Controller != nullptr && AxisValue != 0.0f)
	{
		CurrentVelocity.Y = AxisValue;
		CurrentVelocity.X = 0.f;
		CurrentRotation = Controller->GetControlRotation();
		CurrentRotation.Yaw = AxisValue > 0.0f ? 90.f : 270.f;
		AddMovementInput(CurrentVelocity);
		Controller->SetControlRotation(CurrentRotation);

	}
}

void APacManCharacter::ReStart()
{
	GetWorld()->GetFirstPlayerController()->ConsoleCommand(TEXT("RestartLevel"));
}

void APacManCharacter::NewGame()
{
	if (GameMode->GetCurrentState() == EGameState::EMenu)
		GameMode->SetCurrentState(EGameState::EPlaying);
}

void APacManCharacter::Pause()
{
	if (GameMode->GetCurrentState() == EGameState::EPlaying)
	{
		GameMode->SetCurrentState(EGameState::EPause);
	}
	else if (GameMode->GetCurrentState() == EGameState::EPause)
	{
		GameMode->SetCurrentState(EGameState::EPlaying);
	}
}

void APacManCharacter::Killed()
{
	if (--Lives == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Your life is %d, Game Over"), Lives);
		GameMode->SetCurrentState(EGameState::EGameOver);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Your life is %d"), Lives);
		SetActorLocation(StartPoint);
	}
}

void APacManCharacter::OnCollision(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int OtherBodyIndex, bool bFrowSweep, const FHitResult& SweepResult)
{
	if (GameMode->GetCurrentState() == EGameState::EPlaying)
	{
		if (OtherActor->IsA(ACollectables::StaticClass()))
		{
			ACollectables* collectable = Cast<ACollectables>(OtherActor);
			if (collectable->bIsSupperCollectable)
				GameMode->SetEnemyVulnerable();
			OtherActor->Destroy();
			if (--CollectablesToEat == 0)
				GameMode->SetCurrentState(EGameState::EWin);
			UE_LOG(LogTemp, Warning, TEXT("Remain Collectable is %d"), CollectablesToEat);
		}
	}
}

