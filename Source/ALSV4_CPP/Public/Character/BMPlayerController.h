// Copyright (C) 2020, Doga Can Yanikoglu

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BMPlayerController.generated.h"

class ABMBaseCharacter;

/**
* Player controller class
*/
UCLASS(Blueprintable, BlueprintType)
class ALSV4_CPP_API ABMPlayerController : public APlayerController
{
	GENERATED_BODY()

	void SetupInputComponent() override;

	void OnPossess(APawn* NewPawn) override;

public:
	UFUNCTION(BlueprintCallable, Category = "Essential Information")
	void GetControlForwardRightVector(FVector& Forward, FVector& Right);

	UFUNCTION(BlueprintCallable, Category = "Essential Information")
	FVector GetPlayerMovementInput();

private:
	UPROPERTY(EditDefaultsOnly, Category = "Input", BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float LookUpDownRate = 1.25f;

	UPROPERTY(EditDefaultsOnly, Category = "Input", BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float LookLeftRightRate = 1.25f;

	UPROPERTY(EditDefaultsOnly, Category = "Input", BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float RollDoubleTapTimeout = 0.3f;

	UPROPERTY(EditDefaultsOnly, Category = "Input", BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float ViewModeSwitchHoldTime = 0.2f;

	UPROPERTY(EditDefaultsOnly, Category = "Input", BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	int32 TimesPressedStance = 0;

	UPROPERTY(EditDefaultsOnly, Category = "Input", BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool bBreakFall = false;

	UPROPERTY(EditDefaultsOnly, Category = "Input", BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool bSprintHeld = false;

	/** Main character reference */
	ABMBaseCharacter* PossessedCharacter = nullptr;

	/** Last time the 'first' crouch/roll button is pressed */
	float LastStanceInputTime = 0.0f;

	/** Last time the camera action button is pressed */
	float CameraActionPressedTime = 0.0f;

	/* Timer to manage camera mode swap action */
	FTimerHandle OnCameraModeSwapTimer;

private:
	void PlayerForwardMovementInput(float Value);

	void PlayerRightMovementInput(float Value);

	void PlayerCameraUpInput(float Value);

	void PlayerCameraRightInput(float Value);

	void JumpPressedAction();

	void JumpReleasedAction();

	void SprintPressedAction();

	void SprintReleasedAction();

	void AimPressedAction();

	void AimReleasedAction();

	void CameraPressedAction();

	void CameraReleasedAction();

	void OnSwitchCameraMode();

	void StancePressedAction();

	void WalkPressedAction();

	void RagdollPressedAction();

	void VelocityDirectionPressedAction();

	void LookingDirectionPressedAction();
};
