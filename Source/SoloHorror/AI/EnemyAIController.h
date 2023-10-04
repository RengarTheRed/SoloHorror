// Fill out your copyright notice in the Description page of Project Settings.

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
	
protected:

	// Behavior Tree and Blackboard
	class UBehaviorTreeComponent* BTree;
	class UBlackboardComponent* BB;

	// Sense Configs

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

	// Hearing Config

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class UAISenseConfig_Hearing* HearingConfig;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float AIHearingRange = 800.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float AIHearingAge = 5.f;
};