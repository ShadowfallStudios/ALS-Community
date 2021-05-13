// Project:         Advanced Locomotion System V4 on C++
// Copyright:       Copyright (C) 2021 Doğa Can Yanıkoğlu
// License:         MIT License (http://www.opensource.org/licenses/mit-license.php)
// Source Code:     https://github.com/dyanikoglu/ALSV4_CPP
// Original Author: Jens Bjarne Myhre
// Contributors:    Doğa Can Yanıkoğlu


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
	UBehaviorTree* Behaviour = nullptr;

protected:
	virtual void OnPossess(APawn* InPawn) override;

	virtual FVector GetFocalPointOnActor(const AActor *Actor) const override;
};
