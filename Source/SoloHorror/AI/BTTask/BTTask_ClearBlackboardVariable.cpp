// Copyright 2023 RengarTheRed All rights reserved.


#include "BTTask_ClearBlackboardVariable.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Bool.h"

UBTTask_ClearBlackboardVariable::UBTTask_ClearBlackboardVariable()
{
	bCreateNodeInstance = true;
	NodeName = "Clear Blackboard Variable";
}

// Necessary to Resolve Key before use
void UBTTask_ClearBlackboardVariable::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);
	UBlackboardData* BBAsset = GetBlackboardAsset();
	if (ensure(BBAsset))
	{
		BlackboardKey.ResolveSelectedKey(*BBAsset);
	}
}

EBTNodeResult::Type UBTTask_ClearBlackboardVariable::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// Checks that Blackboard is valid
	UBlackboardComponent* MyBlackboard = OwnerComp.GetBlackboardComponent();
	if(MyBlackboard == nullptr || BlackboardKey.NeedsResolving() == true)
	{
		GEngine->AddOnScreenDebugMessage(0,2,FColor::Green, FString("Bad BB / Clear Var"));
		return EBTNodeResult::Failed;
	}
	
	MyBlackboard->ClearValue(BlackboardKey.SelectedKeyName);
	return EBTNodeResult::Succeeded;
}

