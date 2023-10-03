// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemyAIController.h"

#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"

AEnemyAIController::AEnemyAIController()
{
	PrimaryActorTick.bCanEverTick = true;
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>("Sight Config");
	SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>("Perception"));
}
void AEnemyAIController::BeginPlay()
{
	Super::BeginPlay();
}

void AEnemyAIController::OnPossess(APawn* MyPawn)
{
	Super::OnPossess(MyPawn);

	SetupSenses();
}

void AEnemyAIController::SetupSenses()
{
	// Sight Setup

	SightConfig->SightRadius = AISightRadius;
	SightConfig->LoseSightRadius = AILoseSightRadius;
	SightConfig->PeripheralVisionAngleDegrees = AIFieldOfView;
	SightConfig->SetMaxAge(AISightAge);

	SightConfig->DetectionByAffiliation.bDetectFriendlies=true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals=true;
	SightConfig->DetectionByAffiliation.bDetectEnemies=true;

	GetPerceptionComponent()->SetDominantSense(*SightConfig->GetSenseImplementation());
	GetPerceptionComponent()->OnPerceptionUpdated.AddDynamic(this, &AEnemyAIController::OnPawnDetected);
	GetPerceptionComponent()->ConfigureSense(*SightConfig);

	// Hearing Setup (May not be needed)
}

void AEnemyAIController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void AEnemyAIController::OnPawnDetected(const TArray<AActor*>& DetectedPawns)
{
}

