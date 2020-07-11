// Copyright (C) 2020, Doga Can Yanikoglu
// @author: Jens Bjarne Myhre

#include "Character/AI/ALSAIController.h"

AALSAIController::AALSAIController()
{
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BehaviourDefault(TEXT("/ALSV4_CPP/AdvancedLocomotionV4/Blueprints/CharacterLogic/AI/ALS_BT_AICharacter"));

	Behaviour = BehaviourDefault.Object;
}

void AALSAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if(Behaviour && InPawn)
	{
		RunBehaviorTree(Behaviour);
	}
}
