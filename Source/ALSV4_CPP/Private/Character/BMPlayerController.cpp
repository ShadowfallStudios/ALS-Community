// Copyright (C) 2020, Doga Can Yanikoglu


#include "Character/BMPlayerController.h"
#include "Character/BMCharacter.h"
#include "Character/BMPlayerCameraManager.h"
#include "Kismet/KismetMathLibrary.h"

static TPair<float, float> FixDiagonalGamepadValues(float yIn, float xIn)
{
	float yOut = yIn * FMath::GetMappedRangeValueClamped(FVector2D(0.0f, 0.6f),
	                                                     FVector2D(1.0f, 1.2f), FMath::Abs(xIn));
	yOut = FMath::Clamp(yOut, -1.0f, 1.0f);

	float xOut = xIn * FMath::GetMappedRangeValueClamped(FVector2D(0.0f, 0.6f),
	                                                     FVector2D(1.0f, 1.2f), FMath::Abs(yIn));
	xOut = FMath::Clamp(xOut, -1.0f, 1.0f);

	return TPair<float, float>(yOut, xOut);
}

void ABMPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAxis("MoveForward/Backwards", this, &ABMPlayerController::PlayerForwardMovementInput);
	InputComponent->BindAxis("MoveRight/Left", this, &ABMPlayerController::PlayerRightMovementInput);
	InputComponent->BindAxis("LookUp/Down", this, &ABMPlayerController::PlayerCameraUpInput);
	InputComponent->BindAxis("LookLeft/Right", this, &ABMPlayerController::PlayerCameraRightInput);
	InputComponent->BindAction("JumpAction", IE_Pressed, this, &ABMPlayerController::JumpPressedAction);
	InputComponent->BindAction("JumpAction", IE_Released, this, &ABMPlayerController::JumpReleasedAction);
	InputComponent->BindAction("StanceAction", IE_Pressed, this, &ABMPlayerController::StancePressedAction);
	InputComponent->BindAction("WalkAction", IE_Pressed, this, &ABMPlayerController::WalkPressedAction);
	InputComponent->BindAction("RagdollAction", IE_Pressed, this, &ABMPlayerController::RagdollPressedAction);
	InputComponent->BindAction("SelectRotationMode_1", IE_Pressed, this, &ABMPlayerController::VelocityDirectionPressedAction);
	InputComponent->BindAction("SelectRotationMode_2", IE_Pressed, this, &ABMPlayerController::LookingDirectionPressedAction);
	InputComponent->BindAction("SprintAction", IE_Pressed, this, &ABMPlayerController::SprintPressedAction);
	InputComponent->BindAction("SprintAction", IE_Released, this, &ABMPlayerController::SprintReleasedAction);
	InputComponent->BindAction("AimAction", IE_Pressed, this, &ABMPlayerController::AimPressedAction);
	InputComponent->BindAction("AimAction", IE_Released, this, &ABMPlayerController::AimReleasedAction);
	InputComponent->BindAction("CameraAction", IE_Pressed, this, &ABMPlayerController::CameraPressedAction);
	InputComponent->BindAction("CameraAction", IE_Released, this, &ABMPlayerController::CameraReleasedAction);
}

void ABMPlayerController::OnPawnRestart(APawn* NewPawn)
{
	PossessedCharacter = Cast<ABMBaseCharacter>(NewPawn);
	check(PossessedCharacter);

	// Call "OnPossess" in Player Camera Manager when possessing a pawn
	ABMPlayerCameraManager* CastedMgr = Cast<ABMPlayerCameraManager>(PlayerCameraManager);
	if (CastedMgr)
	{
		CastedMgr->OnPossess(PossessedCharacter);
	}
}

void ABMPlayerController::GetControlForwardRightVector(FVector& Forward, FVector& Right)
{
	const FRotator ControlRot(0.0f, GetControlRotation().Yaw, 0.0f);
	Forward = GetInputAxisValue("MoveForward/Backwards") * UKismetMathLibrary::GetForwardVector(ControlRot);
	Right = GetInputAxisValue("MoveRight/Left") * UKismetMathLibrary::GetRightVector(ControlRot);
}

FVector ABMPlayerController::GetPlayerMovementInput()
{
	FVector Forward;
	FVector Right;
	GetControlForwardRightVector(Forward, Right);
	return (Forward + Right).GetSafeNormal();
}

void ABMPlayerController::PlayerForwardMovementInput(float Value)
{
	if (!PossessedCharacter)
	{
		return;
	}
	
	EBMMovementState MovementState = PossessedCharacter->GetMovementState();
	if (MovementState == EBMMovementState::Grounded || MovementState == EBMMovementState::InAir)
	{
		// Default camera relative movement behavior
		const float Scale = FixDiagonalGamepadValues(Value, GetInputAxisValue("MoveRight/Left")).Key;
		const FRotator DirRotator(0.0f, PossessedCharacter->GetControlRotation().Yaw, 0.0f);
		PossessedCharacter->AddMovementInput(UKismetMathLibrary::GetForwardVector(DirRotator), Scale);
	}
}

void ABMPlayerController::PlayerRightMovementInput(float Value)
{
	if (!PossessedCharacter)
	{
		return;
	}
	
	EBMMovementState MovementState = PossessedCharacter->GetMovementState();
	if (MovementState == EBMMovementState::Grounded || MovementState == EBMMovementState::InAir)
	{
		// Default camera relative movement behavior
		const float Scale = FixDiagonalGamepadValues(GetInputAxisValue("MoveForward/Backwards"), Value).Value;
		const FRotator DirRotator(0.0f, PossessedCharacter->GetControlRotation().Yaw, 0.0f);
		PossessedCharacter->AddMovementInput(UKismetMathLibrary::GetRightVector(DirRotator), Scale);
	}
}

void ABMPlayerController::PlayerCameraUpInput(float Value)
{
	AddPitchInput(LookUpDownRate * Value);
}

void ABMPlayerController::PlayerCameraRightInput(float Value)
{
	AddYawInput(LookLeftRightRate * Value);
}

void ABMPlayerController::JumpPressedAction()
{
	// Jump Action: Press "Jump Action" to end the ragdoll if ragdolling, check for a mantle if grounded or in air,
	// stand up if crouching, or jump if standing.

	if (PossessedCharacter->GetMovementAction() == EBMMovementAction::None)
	{
		if (PossessedCharacter->GetMovementState() == EBMMovementState::Grounded)
		{
			if (PossessedCharacter->HasMovementInput())
			{
				const bool bCanClimbVault = PossessedCharacter->MantleCheckGrounded();
				if (bCanClimbVault)
				{
					return;
				}
			}
			if (PossessedCharacter->GetStance() == EBMStance::Standing)
			{
				PossessedCharacter->Jump();
			}
			else if (PossessedCharacter->GetStance() == EBMStance::Crouching)
			{
				PossessedCharacter->UnCrouch();
			}
		}
		else if (PossessedCharacter->GetMovementState() == EBMMovementState::InAir)
		{
			PossessedCharacter->MantleCheckFalling();
		}
		else if (PossessedCharacter->GetMovementState() == EBMMovementState::Ragdoll)
		{
			PossessedCharacter->RagdollEnd();
		}
	}
}

void ABMPlayerController::JumpReleasedAction()
{
	PossessedCharacter->StopJumping();
}

void ABMPlayerController::SprintPressedAction()
{
	PossessedCharacter->SetDesiredGait(EBMGait::Sprinting);
}

void ABMPlayerController::SprintReleasedAction()
{
	PossessedCharacter->SetDesiredGait(EBMGait::Running);
}

void ABMPlayerController::AimPressedAction()
{
	// AimAction: Hold "AimAction" to enter the aiming mode, release to revert back the desired rotation mode.
	PossessedCharacter->SetRotationMode(EBMRotationMode::Aiming);
}

void ABMPlayerController::AimReleasedAction()
{
	const EBMViewMode ViewMode = PossessedCharacter->GetViewMode();
	if (ViewMode == EBMViewMode::ThirdPerson)
	{
		PossessedCharacter->SetRotationMode(PossessedCharacter->GetDesiredRotationMode());
	}
	else if (ViewMode == EBMViewMode::FirstPerson)
	{
		PossessedCharacter->SetRotationMode(EBMRotationMode::LookingDirection);
	}
}

void ABMPlayerController::CameraPressedAction()
{
	UWorld* World = GetWorld();
	check(World);
	CameraActionPressedTime = World->GetTimeSeconds();
	GetWorldTimerManager().SetTimer(OnCameraModeSwapTimer, this,
	                                &ABMPlayerController::OnSwitchCameraMode, ViewModeSwitchHoldTime, false);
}

void ABMPlayerController::CameraReleasedAction()
{
	if (PossessedCharacter->GetViewMode() == EBMViewMode::FirstPerson)
	{
		// Don't swap shoulders on first person mode
		return;
	}
	
	UWorld* World = GetWorld();
	check(World);
	if (World->GetTimeSeconds() - CameraActionPressedTime < ViewModeSwitchHoldTime)
	{
		// Switch shoulders
		PossessedCharacter->SetRightShoulder(!PossessedCharacter->IsRightShoulder());
		GetWorldTimerManager().ClearTimer(OnCameraModeSwapTimer); // Prevent mode change
	}
}

void ABMPlayerController::OnSwitchCameraMode()
{
	// Switch camera mode
	if (PossessedCharacter->GetViewMode() == EBMViewMode::FirstPerson)
	{
		PossessedCharacter->SetViewMode(EBMViewMode::ThirdPerson);
	}
	else if (PossessedCharacter->GetViewMode() == EBMViewMode::ThirdPerson)
	{
		PossessedCharacter->SetViewMode(EBMViewMode::FirstPerson);
	}
}


void ABMPlayerController::StancePressedAction()
{
	// Stance Action: Press "Stance Action" to toggle Standing / Crouching, double tap to Roll.

	if (PossessedCharacter->GetMovementAction() != EBMMovementAction::None)
	{
		return;
	}

	UWorld* World = GetWorld();
	check(World);

	const float PrevStanceInputTime = LastStanceInputTime;
	LastStanceInputTime = World->GetTimeSeconds();

	if (LastStanceInputTime - PrevStanceInputTime <= RollDoubleTapTimeout)
	{
		// Roll
		PossessedCharacter->OnRoll();
		const EBMStance Stance = PossessedCharacter->GetStance();
		if (Stance == EBMStance::Standing)
		{
			PossessedCharacter->SetDesiredStance(EBMStance::Crouching);
		}
		else if (Stance == EBMStance::Crouching)
		{
			PossessedCharacter->SetDesiredStance(EBMStance::Standing);
		}
		return;
	}

	const EBMMovementState MovementState = PossessedCharacter->GetMovementState();
	if (MovementState == EBMMovementState::Grounded)
	{
		const EBMStance Stance = PossessedCharacter->GetStance();
		if (Stance == EBMStance::Standing)
		{
			PossessedCharacter->SetDesiredStance(EBMStance::Crouching);
			PossessedCharacter->Crouch();
		}
		else if (Stance == EBMStance::Crouching)
		{
			PossessedCharacter->SetDesiredStance(EBMStance::Standing);
			PossessedCharacter->UnCrouch();
		}
	}

	// Notice: MovementState == EBMMovementState::InAir case is removed
}

void ABMPlayerController::WalkPressedAction()
{
	const EBMGait DesiredGait = PossessedCharacter->GetDesiredGait();
	if (DesiredGait == EBMGait::Walking)
	{
		PossessedCharacter->SetDesiredGait(EBMGait::Running);
	}
	else if (DesiredGait == EBMGait::Running)
	{
		PossessedCharacter->SetDesiredGait(EBMGait::Walking);
	}
}

void ABMPlayerController::RagdollPressedAction()
{
	// Ragdoll Action: Press "Ragdoll Action" to toggle the ragdoll state on or off.

	if (PossessedCharacter->GetMovementState() == EBMMovementState::Ragdoll)
	{
		PossessedCharacter->RagdollEnd();
	}
	else
	{
		PossessedCharacter->RagdollStart();
	}
}

void ABMPlayerController::VelocityDirectionPressedAction()
{
	// Select Rotation Mode: Switch the desired (default) rotation mode to Velocity or Looking Direction.
	// This will be the mode the character reverts back to when un-aiming

	PossessedCharacter->SetDesiredRotationMode(EBMRotationMode::VelocityDirection);
	PossessedCharacter->SetRotationMode(EBMRotationMode::VelocityDirection);
}

void ABMPlayerController::LookingDirectionPressedAction()
{
	PossessedCharacter->SetDesiredRotationMode(EBMRotationMode::LookingDirection);
	PossessedCharacter->SetRotationMode(EBMRotationMode::LookingDirection);
}
