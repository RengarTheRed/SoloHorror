// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemyAIController.h"

#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "Perception/AISenseConfig_Sight.h"

AEnemyAIController::AEnemyAIController()
{
	PrimaryActorTick.bCanEverTick = true;

	// Creates Base Perception Comps
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>("Sight Config");
	HearingConfig = CreateDefaultSubobject<UAISenseConfig_Hearing>("Hearing Config");
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

	// Hearing Setup
	
	HearingConfig->HearingRange = AIHearingRange;
	HearingConfig->SetMaxAge(AIHearingAge);
	
	HearingConfig->DetectionByAffiliation.bDetectFriendlies=true;
	HearingConfig->DetectionByAffiliation.bDetectNeutrals=true;
	HearingConfig->DetectionByAffiliation.bDetectEnemies=true;
}

void AEnemyAIController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void AEnemyAIController::OnPawnDetected(const TArray<AActor*>& DetectedPawns)
{

	// Handles Heard Actors first
	
	FAISenseID HearingSenseID = UAISense::GetSenseID<UAISense_Hearing>(); 	
	if (PerceptionComponent->GetSenseConfig(HearingSenseID) != nullptr)
	{
		const FActorPerceptionInfo* HeardPerceptionInfo = PerceptionComponent->GetFreshestTrace(HearingSenseID);
		if (HeardPerceptionInfo != nullptr && PerceptionComponent->HasActiveStimulus(*HeardPerceptionInfo->Target, HearingSenseID))
		{
			FVector HeardSomethingLocation = HeardPerceptionInfo->GetStimulusLocation(HearingSenseID);
		}
	}

	// Sight
	
	FAISenseID SightSenseID = UAISense::GetSenseID<UAISenseConfig_Sight>(); 	
	if (PerceptionComponent->GetSenseConfig(SightSenseID) != nullptr)
	{
		const FActorPerceptionInfo* SightPerceptionInfo = PerceptionComponent->GetFreshestTrace(SightSenseID);
		if (SightPerceptionInfo != nullptr && PerceptionComponent->HasActiveStimulus(*SightPerceptionInfo->Target, SightSenseID))
		{
			FVector SeenSomethingLocation = SightPerceptionInfo->GetStimulusLocation(SightSenseID);
		}
	}

	
}

