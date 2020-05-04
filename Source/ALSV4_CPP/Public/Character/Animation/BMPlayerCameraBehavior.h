// Copyright (C) 2020, Doga Can Yanikoglu

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Library/BMCharacterEnumLibrary.h"

#include "BMPlayerCameraBehavior.generated.h"

class ABMBaseCharacter;
class ABMPlayerController;

/**
 * Main class for player camera movement behavior
 */
UCLASS(Blueprintable, BlueprintType)
class ALSV4_CPP_API UBMPlayerCameraBehavior : public UAnimInstance
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	ABMBaseCharacter* ControlledPawn = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	APlayerController* PlayerController = nullptr;

protected:
	void NativeUpdateAnimation(float DeltaSeconds) override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	EBMMovementState MovementState;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	EBMMovementAction MovementAction;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	EBMRotationMode RotationMode;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	EBMGait Gait;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	EBMStance Stance;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	EBMViewMode ViewMode;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bRightShoulder;
};
