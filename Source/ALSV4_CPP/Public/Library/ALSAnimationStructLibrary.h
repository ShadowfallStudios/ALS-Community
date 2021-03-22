// Project:         Advanced Locomotion System V4 on C++
// Copyright:       Copyright (C) 2021 Doğa Can Yanıkoğlu
// License:         MIT License (http://www.opensource.org/licenses/mit-license.php)
// Source Code:     https://github.com/dyanikoglu/ALSV4_CPP
// Original Author: Doğa Can Yanıkoğlu
// Contributors:    


#pragma once

#include "CoreMinimal.h"
#include "Runtime/Engine/Classes/Animation/AnimSequenceBase.h"
#include "ALSCharacterEnumLibrary.h"


#include "ALSAnimationStructLibrary.generated.h"


USTRUCT(BlueprintType)
struct FALSDynamicMontageParams
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimSequenceBase* Animation = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BlendInTime = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BlendOutTime = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float PlayRate = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float StartTime = 0.0f;
};

USTRUCT(BlueprintType)
struct FALSLeanAmount
{
	GENERATED_BODY()

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	float LR = 0.0f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	float FB = 0.0f;
};

USTRUCT(BlueprintType)
struct FALSVelocityBlend
{
	GENERATED_BODY()

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	float F = 0.0f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	float B = 0.0f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	float L = 0.0f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	float R = 0.0f;
};

USTRUCT(BlueprintType)
struct FALSTurnInPlaceAsset
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UAnimSequenceBase* Animation = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float AnimatedAngle = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName SlotName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float PlayRate = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool ScaleTurnAngle = true;
};

USTRUCT(BlueprintType)
struct FALSAnimCharacterInformation
{
	GENERATED_BODY()

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	FRotator AimingRotation;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	FRotator CharacterActorRotation;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	FVector Velocity;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	FVector RelativeVelocityDirection;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	FVector Acceleration;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	FVector MovementInput;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	bool bIsMoving = false;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	bool bHasMovementInput = false;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	float Speed = 0.0f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	float MovementInputAmount = 0.0f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	float AimYawRate = 0.0f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	float ZoomAmount = 0.0f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	EALSMovementState PrevMovementState = EALSMovementState::None;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	EALSViewMode ViewMode = EALSViewMode::ThirdPerson;
};


USTRUCT(BlueprintType)
struct FALSAnimGraphGrounded
{
	GENERATED_BODY()

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite)
	EALSHipsDirection TrackedHipsDirection = EALSHipsDirection::F;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	bool bShouldMove = false; // Should be false initially

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	bool bRotateL = false;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	bool bRotateR = false;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite)
	bool bPivot = false;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	float RotateRate = 1.0f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	float RotationScale = 0.0f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	float DiagonalScaleAmount = 0.0f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	float WalkRunBlend = 0.0f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	float StandingPlayRate = 1.0f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	float CrouchingPlayRate = 1.0f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	float StrideBlend = 0.0f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	float FYaw = 0.0f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	float BYaw = 0.0f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	float LYaw = 0.0f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	float RYaw = 0.0f;
};

USTRUCT(BlueprintType)
struct FALSAnimGraphInAir
{
	GENERATED_BODY()

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	bool bJumped = false;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	float JumpPlayRate = 1.2f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	float FallSpeed = 0.0f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	float LandPrediction = 1.0f;
};

USTRUCT(BlueprintType)
struct FALSAnimGraphAimingValues
{
	GENERATED_BODY()

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	FRotator SmoothedAimingRotation;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	FRotator SpineRotation;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	FVector2D AimingAngle;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	float AimSweepTime = 0.5f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	float InputYawOffsetTime = 0.0f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	float ForwardYawTime = 0.0f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	float LeftYawTime = 0.0f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	float RightYawTime = 0.0f;
};

USTRUCT(BlueprintType)
struct FALSAnimGraphLayerBlending
{
	GENERATED_BODY()

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	int32 OverlayOverrideState = 0;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	float EnableAimOffset = 1.0f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	float BasePose_N = 1.0f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	float BasePose_CLF = 0.0f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	float Arm_L = 0.0f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	float Arm_L_Add = 0.0f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	float Arm_L_LS = 0.0f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	float Arm_L_MS = 0.0f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	float Arm_R = 0.0f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	float Arm_R_Add = 0.0f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	float Arm_R_LS = 0.0f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	float Arm_R_MS = 0.0f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	float Hand_L = 0.0f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	float Hand_R = 0.0f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	float Legs = 0.0f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	float Legs_Add = 0.0f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	float Pelvis = 0.0f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	float Pelvis_Add = 0.0f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	float Spine = 0.0f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	float Spine_Add = 0.0f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	float Head = 0.0f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	float Head_Add = 0.0f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	float EnableHandIK_L = 1.0f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	float EnableHandIK_R = 1.0f;
};

USTRUCT(BlueprintType)
struct FALSAnimGraphFootIK
{
	GENERATED_BODY()

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	float FootLock_L_Alpha = 0.0f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	float FootLock_R_Alpha = 0.0f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	bool UseFootLockCurve_L;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	bool UseFootLockCurve_R;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	FVector FootLock_L_Location;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	FVector TargetFootLock_R_Location;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	FVector FootLock_R_Location;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	FRotator TargetFootLock_L_Rotation;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	FRotator FootLock_L_Rotation;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	FRotator TargetFootLock_R_Rotation;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	FRotator FootLock_R_Rotation;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	FVector FootOffset_L_Location;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	FVector FootOffset_R_Location;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	FRotator FootOffset_L_Rotation;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	FRotator FootOffset_R_Rotation;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	FVector PelvisOffset;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	float PelvisAlpha = 0.0f;
};

USTRUCT(BlueprintType)
struct FALSAnimTurnInPlace
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float TurnCheckMinAngle = 45.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Turn180Threshold = 130.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float AimYawRateLimit = 50.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float ElapsedDelayTime = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MinAngleDelay = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MaxAngleDelay = 0.75f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FALSTurnInPlaceAsset N_TurnIP_L90;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FALSTurnInPlaceAsset N_TurnIP_R90;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FALSTurnInPlaceAsset N_TurnIP_L180;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FALSTurnInPlaceAsset N_TurnIP_R180;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FALSTurnInPlaceAsset CLF_TurnIP_L90;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FALSTurnInPlaceAsset CLF_TurnIP_R90;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FALSTurnInPlaceAsset CLF_TurnIP_L180;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FALSTurnInPlaceAsset CLF_TurnIP_R180;
};

USTRUCT(BlueprintType)
struct FALSAnimRotateInPlace
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float RotateMinThreshold = -50.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float RotateMaxThreshold = 50.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float AimYawRateMinRange = 90.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float AimYawRateMaxRange = 270.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MinPlayRate = 1.15f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MaxPlayRate = 3.0f;
};

USTRUCT(BlueprintType)
struct FALSAnimConfiguration
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float AnimatedWalkSpeed = 150.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float AnimatedRunSpeed = 350.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float AnimatedSprintSpeed = 600.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float AnimatedCrouchSpeed = 150.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float VelocityBlendInterpSpeed = 12.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float GroundedLeanInterpSpeed = 4.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float InAirLeanInterpSpeed = 4.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float SmoothedAimingRotationInterpSpeed = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float InputYawOffsetInterpSpeed = 8.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float TriggerPivotSpeedLimit = 200.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float FootHeight = 13.5f;

	/** Threshold value for activating dynamic transition on various animations */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float DynamicTransitionThreshold = 8.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float IK_TraceDistanceAboveFoot = 50.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float IK_TraceDistanceBelowFoot = 45.0f;
};
