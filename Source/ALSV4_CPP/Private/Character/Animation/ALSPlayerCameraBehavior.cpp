// Project:         Advanced Locomotion System V4 on C++
// Copyright:       Copyright (C) 2020 Doğa Can Yanıkoğlu
// License:         MIT License (http://www.opensource.org/licenses/mit-license.php)
// Source Code:     https://github.com/dyanikoglu/ALSV4_CPP
// Original Author: Doğa Can Yanıkoğlu
// Contributors:    


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
