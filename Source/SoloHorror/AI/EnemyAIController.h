// Copyright 2023 RengarTheRed All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyAIController.generated.h"


UCLASS()
class SOLOHORROR_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()
	
	AEnemyAIController();

	void SetupSenses();


public:
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* MyPawn) override;
	virtual void Tick(float DeltaSeconds) override;

	UFUNCTION()
	void OnPawnDetected(const TArray<AActor*>& DetectedPawns);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UBehaviorTree* TreeAsset;
	
	// Behavior Tree and Blackboard
	class UBehaviorTreeComponent* BTree;
	UBlackboardComponent* BB;

	// Sense Configs
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UAIPerceptionComponent* MyPerceptionComp;

	// Sight Config
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class UAISenseConfig_Sight* SightConfig;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float AISightRadius = 500.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float AISightAge = 5.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float AILoseSightRadius = AISightRadius + 50.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float AIFieldOfView = 90.f;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float DistanceToPlayer = 0.f;

	// Sight Timer Setup
	
	FTimerHandle SightTimer;
	AActor* PlayerActor;
	void TimerUpdate();
	float TimerRunTime = 0.f;

	// Hearing Config

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class UAISenseConfig_Hearing* HearingConfig;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float AIHearingRange = 800.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float AIHearingAge = 5.f;
};
