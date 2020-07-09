// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "ALS_BTTask_SetFocusToPlayer.generated.h"

/**
 * 
 */
UCLASS(Category = ALS, meta = (DisplayName = "Set Focus to Player"))
class ALSV4_CPP_API UALS_BTTask_SetFocusToPlayer : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UALS_BTTask_SetFocusToPlayer();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual FString GetStaticDescription() const override;
};
