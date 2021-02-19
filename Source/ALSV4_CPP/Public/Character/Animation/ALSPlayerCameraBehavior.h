// Project:         Advanced Locomotion System V4 on C++
// Copyright:       Copyright (C) 2021 Doğa Can Yanıkoğlu
// License:         MIT License (http://www.opensource.org/licenses/mit-license.php)
// Source Code:     https://github.com/dyanikoglu/ALSV4_CPP
// Original Author: Doğa Can Yanıkoğlu
// Contributors:    


#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Library/ALSCharacterEnumLibrary.h"

#include "ALSPlayerCameraBehavior.generated.h"

class AALSBaseCharacter;
class AALSPlayerController;

/**
 * Main class for player camera movement behavior
 */
UCLASS(Blueprintable, BlueprintType)
class ALSV4_CPP_API UALSPlayerCameraBehavior : public UAnimInstance
{
	GENERATED_BODY()

public:
	void SetRotationMode(EALSRotationMode RotationMode);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	EALSMovementState MovementState;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	EALSMovementAction MovementAction;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bLookingDirection = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bVelocityDirection = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bAiming = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	EALSGait Gait;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	EALSStance Stance;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	EALSViewMode ViewMode;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bRightShoulder = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bDebugView = false;
};
