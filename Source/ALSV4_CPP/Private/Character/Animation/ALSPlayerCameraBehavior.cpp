// Copyright (C) 2020, Doga Can Yanikoglu


#include "Character/Animation/ALSPlayerCameraBehavior.h"


#include "Character/ALSBaseCharacter.h"

void UALSPlayerCameraBehavior::NativeUpdateAnimation(float DeltaSeconds)
{
	if (ControlledPawn)
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
