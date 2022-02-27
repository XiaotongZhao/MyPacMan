// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "PacManCharacter.h"
#include "AIEnemy.h"

// Sets default values
AEnemy::AEnemy()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	static ConstructorHelpers::FObjectFinder<UStaticMesh> CylinderObj(TEXT("'/Game/StarterContent/Shapes/Shape_Cylinder'"));

	GetCapsuleComponent()->SetCapsuleRadius(40.0f);
	GetCapsuleComponent()->SetCapsuleHalfHeight(50.0f);
	static ConstructorHelpers::FObjectFinder<UMaterial>VulnerableMat(TEXT("'/Game/Materials/M_Enemy_Vulnerable'"));
	
	
	VulnerableMaterial = VulnerableMat.Object;
	EnemyMesh = this->GetMesh();

	SetActorEnableCollision(true);
	AIControllerClass = AAIEnemy::StaticClass();


}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	DefaultMaterial = EnemyMesh->GetMaterial(1);
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::OnCollision);
	GetCharacterMovement()->MaxWalkSpeed = 150.0f;
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemy::SetVulnerable()
{
	GetWorldTimerManager().SetTimer(TimeVulnerable, this, &AEnemy::SetInVulnerable, 5.0f, false);
	if (bIsVulnerable)
		return;
	bIsVulnerable = true;
	EnemyMesh->SetMaterial(1, VulnerableMaterial);
	GetCharacterMovement()->MaxWalkSpeed = 50.0f;
	UE_LOG(LogTemp, Warning, TEXT("bIsVulnerable is %d"), bIsVulnerable);
}

void AEnemy::SetInVulnerable()
{
	GetWorldTimerManager().ClearTimer(TimeVulnerable);
	bIsVulnerable = false;
	EnemyMesh->SetMaterial(1, DefaultMaterial);
	GetCharacterMovement()->MaxWalkSpeed = 150.0f;
	UE_LOG(LogTemp, Warning, TEXT("bIsVulnerable is %d"), bIsVulnerable);
}

void AEnemy::SetMove(bool bMoveIt)
{
	AAIEnemy* AI = Cast<AAIEnemy>(GetController());

	if (bMoveIt)
		AI->SearchNewPoint();
	else
		AI->StopMove();
}

void AEnemy::Killed()
{
	if (bIsDead)
		return;
	bIsDead = true;
	GetCharacterMovement()->MaxWalkSpeed = 300.0f;
	AAIEnemy* AI = Cast<AAIEnemy>(GetController());
	AI->GoHome();
}

void AEnemy::ReArm()
{
	bIsDead = false;
	GetCharacterMovement()->MaxWalkSpeed = 150.0f;
	if (bIsVulnerable)
		SetInVulnerable();
	SetMove(true);
}

void AEnemy::OnCollision(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int OtherBodyIndex, bool bFrowSweep, const FHitResult& SweepResult)
{
	if (OtherActor->IsA(APacManCharacter::StaticClass()))
	{
		if (bIsVulnerable)
			Killed();
		else
		{
			APacManCharacter* PacMan = Cast<APacManCharacter>(OtherActor);
			PacMan->Killed();
		}
	}
}

