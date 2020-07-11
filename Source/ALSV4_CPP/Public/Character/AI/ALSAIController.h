// Copyright (C) 2020, Doga Can Yanikoglu
// @author: Jens Bjarne Myhre

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Runtime/AIModule/Classes/BehaviorTree/BehaviorTree.h"
#include "ALSAIController.generated.h"

/**
 * 
 */
UCLASS()
class ALSV4_CPP_API AALSAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	AALSAIController();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AI")
	UBehaviorTree* Behaviour;

protected:
	virtual void OnPossess(APawn* InPawn) override;
};
