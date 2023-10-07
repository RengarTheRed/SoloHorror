// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemyAIController.h"

#include <string>

#include "EnemyCharacter.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "Perception/AISenseConfig_Sight.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Bool.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"

AEnemyAIController::AEnemyAIController()
{
	PrimaryActorTick.bCanEverTick = true;

	// Create Blackboard and BTree Components
	BB = CreateDefaultSubobject<UBlackboardComponent>("Blackboard");
	BTree = CreateDefaultSubobject<UBehaviorTreeComponent>("Behaviour Tree");
	
	// Creates Base Perception Comps
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>("Sight Config");
	HearingConfig = CreateDefaultSubobject<UAISenseConfig_Hearing>("Hearing Config");
	MyPerceptionComp = CreateDefaultSubobject<UAIPerceptionComponent>("Perception");
	
	// Ensure PerceptionComp exists before Configuring Senses
	if(MyPerceptionComp!=nullptr)
	{
		SetupSenses();
	}
}
void AEnemyAIController::BeginPlay()
{
	Super::BeginPlay();
}

void AEnemyAIController::OnPossess(APawn* MyPawn)
{
	Super::OnPossess(MyPawn);

	// Ensures Controlled Character and TreeAsset are set before starting
	AEnemyCharacter* MyCharacter = Cast<AEnemyCharacter>(MyPawn);
	if (MyCharacter && TreeAsset)
	{
		BB->InitializeBlackboard(*TreeAsset->BlackboardAsset);
		BTree->StartTree(*TreeAsset);
	}
	
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


	// Hearing Setup
	
	HearingConfig->HearingRange = AIHearingRange;
	HearingConfig->SetMaxAge(AIHearingAge);
	
	HearingConfig->DetectionByAffiliation.bDetectFriendlies=true;
	HearingConfig->DetectionByAffiliation.bDetectNeutrals=true;
	HearingConfig->DetectionByAffiliation.bDetectEnemies=true;

	// Adds Sense Configs to Perception Component
	MyPerceptionComp->ConfigureSense(*SightConfig);
	MyPerceptionComp->ConfigureSense(*HearingConfig);
	MyPerceptionComp->SetDominantSense(*SightConfig->GetSenseImplementation());
	MyPerceptionComp->OnPerceptionUpdated.AddDynamic(this, &AEnemyAIController::OnPawnDetected);
}

void AEnemyAIController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if(DistanceToPlayer>AILoseSightRadius)
	{
		GEngine->AddOnScreenDebugMessage(0,5.f, FColor::Green, FString("Cleared Player Vars"));
		BB->ClearValue("SeePlayer");
		BB->ClearValue("Player");
	}
}

void AEnemyAIController::OnPawnDetected(const TArray<AActor*>& DetectedPawns)
{
	GEngine->AddOnScreenDebugMessage(0, 4.f, FColor::Blue, FString("Perceived"));

	
	// Hearing
	FAISenseID HearingSenseID = UAISense::GetSenseID<UAISense_Hearing>(); 	
	if (PerceptionComponent->GetSenseConfig(HearingSenseID) != nullptr)
	{
		const FActorPerceptionInfo* HeardPerceptionInfo = PerceptionComponent->GetFreshestTrace(HearingSenseID);
		if (HeardPerceptionInfo != nullptr && PerceptionComponent->HasActiveStimulus(*HeardPerceptionInfo->Target, HearingSenseID))
		{
			if(HeardPerceptionInfo->Target->ActorHasTag("Player"))
			{
				FVector HeardSomethingLocation = HeardPerceptionInfo->GetStimulusLocation(HearingSenseID);
				GEngine->AddOnScreenDebugMessage(0, 5.f, FColor::Green, FString("Player Heard"));
				BB->SetValue<UBlackboardKeyType_Bool>("HeardSound", true);
				BB->SetValue<UBlackboardKeyType_Bool>("ChasingPlayer", true);
				BB->SetValue<UBlackboardKeyType_Vector>("LastSoundLocation", HeardSomethingLocation);
			}
		}
	}

	// Sight
	
	TArray<AActor*> SeenActors;
	GetPerceptionComponent()->GetCurrentlyPerceivedActors(GetPerceptionComponent()->GetDominantSense(), SeenActors);

	for (auto SeenActor : SeenActors)
	{
		if(SeenActor->ActorHasTag("Player"))
		{
			GEngine->AddOnScreenDebugMessage(0, 5.f, FColor::Green, FString("Player Sighted"));
			BB->SetValue<UBlackboardKeyType_Bool>("SeePlayer", true);
			BB->SetValue<UBlackboardKeyType_Bool>("ChasingPlayer", true);
			BB->SetValue<UBlackboardKeyType_Object>("Player", SeenActor);
			BB->SetValue<UBlackboardKeyType_Vector>("LastSeenPlayerLocation", SeenActor->GetActorLocation());
			DistanceToPlayer = GetPawn()->GetDistanceTo(SeenActor);
			GEngine->AddOnScreenDebugMessage(0, 5.f, FColor::Green, FString::SanitizeFloat(DistanceToPlayer));
		}
	}	

}

