/** Copyright (c) 2020 Doğa Can YANIKOĞLU */


#include "Character/Animation/BMPlayerCameraBehavior.h"


#include "Character/BMBaseCharacter.h"

void UBMPlayerCameraBehavior::NativeUpdateAnimation(float DeltaSeconds)
{
	if (IsValid(ControlledPawn))
	{
		MovementState = ControlledPawn->GetMovementState();
		MovementAction = ControlledPawn->GetMovementAction();
		RotationMode = ControlledPawn->GetRotationMode();
		Gait = ControlledPawn->GetGait();
		Stance = ControlledPawn->GetStance();
		ViewMode = ControlledPawn->GetViewMode();
		bRightShoulder = ControlledPawn->IsRightShoulder();
	}
}
