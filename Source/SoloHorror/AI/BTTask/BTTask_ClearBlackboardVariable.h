// Copyright 2023 RengarTheRed All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_ClearBlackboardVariable.generated.h"

/**
 * 
 */
UCLASS()
class SOLOHORROR_API UBTTask_ClearBlackboardVariable : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	UBTTask_ClearBlackboardVariable();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;
};