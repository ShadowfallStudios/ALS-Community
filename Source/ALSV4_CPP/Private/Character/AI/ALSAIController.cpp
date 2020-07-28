// Project:         Advanced Locomotion System V4 on C++
// Copyright:       Copyright (C) 2020 Doğa Can Yanıkoğlu
// License:         MIT License (http://www.opensource.org/licenses/mit-license.php)
// Source Code:     https://github.com/dyanikoglu/ALSV4_CPP
// Original Author: Jens Bjarne Myhre
// Contributors:    

#include "Character/AI/ALSAIController.h"
#include "UObject/ConstructorHelpers.h"

AALSAIController::AALSAIController()
{
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BehaviourDefault(
		TEXT("/ALSV4_CPP/AdvancedLocomotionV4/Blueprints/CharacterLogic/AI/ALS_BT_AICharacter"));

	Behaviour = BehaviourDefault.Object;
}

void AALSAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (Behaviour && InPawn)
	{
		RunBehaviorTree(Behaviour);
	}
}
