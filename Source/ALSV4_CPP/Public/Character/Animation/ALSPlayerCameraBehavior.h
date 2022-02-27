// Copyright:       Copyright (C) 2022 Doğa Can Yanıkoğlu
// Source Code:     https://github.com/dyanikoglu/ALS-Community


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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Read Only Data|Character Information")
	EALSMovementState MovementState;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Read Only Data|Character Information")
	EALSMovementAction MovementAction;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Read Only Data|Character Information")
	bool bLookingDirection = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Read Only Data|Character Information")
	bool bVelocityDirection = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Read Only Data|Character Information")
	bool bAiming = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Read Only Data|Character Information")
	EALSGait Gait;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Read Only Data|Character Information")
	EALSStance Stance;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Read Only Data|Character Information")
	EALSViewMode ViewMode;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Read Only Data|Character Information")
	bool bRightShoulder = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Read Only Data|Character Information")
	bool bDebugView = false;
};
