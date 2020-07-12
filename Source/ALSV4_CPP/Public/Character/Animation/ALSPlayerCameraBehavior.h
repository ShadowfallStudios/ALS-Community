// Copyright (C) 2020, Doga Can Yanikoglu

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
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	AALSBaseCharacter* ControlledPawn = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	APlayerController* PlayerController = nullptr;

protected:
	void NativeUpdateAnimation(float DeltaSeconds) override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	EALSMovementState MovementState;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	EALSMovementAction MovementAction;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	EALSRotationMode RotationMode;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	EALSGait Gait;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	EALSStance Stance;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	EALSViewMode ViewMode;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bRightShoulder;
};
