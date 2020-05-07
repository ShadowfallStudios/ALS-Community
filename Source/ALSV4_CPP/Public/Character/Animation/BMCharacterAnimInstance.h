// Copyright (C) 2020, Doga Can Yanikoglu

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Library/BMCharacterEnumLibrary.h"

#include "BMCharacterAnimInstance.generated.h"

class ABMBaseCharacter;
class UCurveFloat;
class UAnimSequence;
class UCurveVector;

USTRUCT(BlueprintType)
struct FBMDynamicMontageParams
{
	GENERATED_USTRUCT_BODY()

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
struct FBMLeanAmount
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	float LR = 0.0f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	float FB = 0.0f;
};

USTRUCT(BlueprintType)
struct FBMVelocityBlend
{
	GENERATED_USTRUCT_BODY()

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
struct FBMTurnInPlaceAsset
{
	GENERATED_USTRUCT_BODY()

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

/**
 * Main anim instance class for character
 */
UCLASS(Blueprintable, BlueprintType)
class ALSV4_CPP_API UBMCharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

	void NativeInitializeAnimation() override;

	void NativeUpdateAnimation(float DeltaSeconds) override;

	UFUNCTION(BlueprintCallable)
	void PlayTransition(const FBMDynamicMontageParams& Parameters);

	UFUNCTION(BlueprintCallable)
	void PlayTransitionChecked(const FBMDynamicMontageParams& Parameters);

	UFUNCTION(BlueprintCallable)
	void PlayDynamicTransition(float ReTriggerDelay, FBMDynamicMontageParams Parameters);

public:
	UFUNCTION(BlueprintCallable)
	void OnJumped();

	UFUNCTION(BlueprintCallable)
	void OnPivot();

	/** Enable Movement Animations if IsMoving and HasMovementInput, or if the Speed is greater than 150. */
	UFUNCTION(BlueprintCallable, Category = "Grounded")
	bool ShouldMoveCheck();

	/** Only perform a Rotate In Place Check if the character is Aiming or in First Person. */
	UFUNCTION(BlueprintCallable, Category = "Grounded")
	bool CanRotateInPlace();

	/**
	 * Only perform a Turn In Place check if the character is looking toward the camera in Third Person,
	 * and if the "Enable Transition" curve is fully weighted. The Enable_Transition curve is modified within certain
	 * states of the AnimBP so that the character can only turn while in those states..
	 */
	UFUNCTION(BlueprintCallable, Category = "Grounded")
	bool CanTurnInPlace();

	/**
	 * Only perform a Dynamic Transition check if the "Enable Transition" curve is fully weighted.
	 * The Enable_Transition curve is modified within certain states of the AnimBP so
	 * that the character can only transition while in those states.
	 */
	UFUNCTION(BlueprintCallable, Category = "Grounded")
	bool CanDynamicTransition();

private:
	void PlayDynamicTransitionDelay();

	void OnJumpedDelay();

	void OnPivotDelay();

	/** Update Values */

	void UpdateAimingValues(float DeltaSeconds);

	void UpdateLayerValues();

	void UpdateFootIK(float DeltaSeconds);

	void UpdateMovementValues(float DeltaSeconds);

	void UpdateRotationValues();

	void UpdateInAirValues(float DeltaSeconds);

	void UpdateRagdollValues();

	/** Foot IK */

	void SetFootLocking(float DeltaSeconds, FName EnableFootIKCurve, FName FootLockCurve, FName IKFootBone,
	                    float& CurFootLockAlpha, FVector& CurFootLockLoc, FRotator& CurFootLockRot);

	void SetFootLockOffsets(float DeltaSeconds, FVector& LocalLoc, FRotator& LocalRot);

	void SetPelvisIKOffset(float DeltaSeconds, FVector FootOffsetLTarget, FVector FootOffsetRTarget);

	void ResetIKOffsets(float DeltaSeconds);

	void SetFootOffsets(float DeltaSeconds, FName EnableFootIKCurve, FName IKFootBone, FName RootBone,
	                    FVector& CurLocationTarget, FVector& CurLocationOffset, FRotator& CurRotationOffset);

	/** Grounded */

	void RotateInPlaceCheck();

	void TurnInPlaceCheck(float DeltaSeconds);

	void DynamicTransitionCheck();

	FBMVelocityBlend CalculateVelocityBlend();

	void TurnInPlace(FRotator TargetRotation, float PlayRateScale, float StartTime, bool OverrideCurrent);

	/** Movement */

	FVector CalculateRelativeAccelerationAmount();

	float CalculateStrideBlend();

	float CalculateWalkRunBlend();

	float CalculateStandingPlayRate();

	float CalculateDiagonalScaleAmount();

	float CalculateCrouchingPlayRate();

	float CalculateLandPrediction();

	FBMLeanAmount CalculateAirLeanAmount();

	EBMMovementDirection CalculateMovementDirection();

	EBMMovementDirection CalculateQuadrant(EBMMovementDirection Current, float FRThreshold, float FLThreshold, float BRThreshold,
	                                       float BLThreshold, float Buffer, float Angle);

	/** Util */

	float GetAnimCurveClamped(const FName& Name, float Bias, float ClampMin, float ClampMax);

public:
	/** References */
	UPROPERTY(BlueprintReadWrite)
	ABMBaseCharacter* Character = nullptr;

	/** Character Information */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Read Only Data|Character Information")
	FRotator AimingRotation;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Read Only Data|Character Information")
	FRotator CharacterActorRotation;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Read Only Data|Character Information")
	FVector Velocity;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Read Only Data|Character Information")
	FVector RelativeVelocityDirection;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Read Only Data|Character Information")
	FVector Acceleration;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Read Only Data|Character Information")
	FVector MovementInput;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Read Only Data|Character Information")
	bool bIsMoving = false;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Read Only Data|Character Information")
	bool bHasMovementInput = false;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Read Only Data|Character Information")
	float Speed = 0.0f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Read Only Data|Character Information")
	float MovementInputAmount = 0.0f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Read Only Data|Character Information")
	float AimYawRate = 0.0f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Read Only Data|Character Information")
	float ZoomAmount = 0.0f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Read Only Data|Character Information")
	EBMMovementState MovementState = EBMMovementState::None;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Read Only Data|Character Information")
	EBMMovementState PrevMovementState = EBMMovementState::None;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Read Only Data|Character Information")
	EBMMovementAction MovementAction = EBMMovementAction::None;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Read Only Data|Character Information")
	EBMRotationMode RotationMode = EBMRotationMode::LookingDirection;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Read Only Data|Character Information")
	EBMGait Gait = EBMGait::Walking;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Read Only Data|Character Information")
	EBMStance Stance = EBMStance::Standing;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Read Only Data|Character Information")
	EBMViewMode ViewMode = EBMViewMode::ThirdPerson;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Read Only Data|Character Information")
	EBMOverlayState OverlayState = EBMOverlayState::Default;

	/** Anim Graph - Grounded */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Read Only Data|Anim Graph - Grounded")
	EBMGroundedEntryState GroundedEntryState = EBMGroundedEntryState::None;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Read Only Data|Anim Graph - Grounded")
	EBMMovementDirection MovementDirection = EBMMovementDirection::Forward;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Read Only Data|Anim Graph - Grounded")
	EBMHipsDirection TrackedHipsDirection = EBMHipsDirection::F;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Read Only Data|Anim Graph - Grounded")
	FVector RelativeAccelerationAmount;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Read Only Data|Anim Graph - Grounded")
	bool bShouldMove = false; // Should be false initially

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Read Only Data|Anim Graph - Grounded")
	bool bRotateL = false;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Read Only Data|Anim Graph - Grounded")
	bool bRotateR = false;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Read Only Data|Anim Graph - Grounded")
	bool bPivot = false;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Read Only Data|Anim Graph - Grounded")
	float RotateRate = 1.0f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Read Only Data|Anim Graph - Grounded")
	float RotationScale = 0.0f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Read Only Data|Anim Graph - Grounded")
	float DiagonalScaleAmount = 0.0f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Read Only Data|Anim Graph - Grounded")
	float WalkRunBlend = 0.0f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Read Only Data|Anim Graph - Grounded")
	float StandingPlayRate = 1.0f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Read Only Data|Anim Graph - Grounded")
	float CrouchingPlayRate = 1.0f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Read Only Data|Anim Graph - Grounded")
	float StrideBlend = 0.0f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Read Only Data|Anim Graph - Grounded")
	FBMVelocityBlend VelocityBlend;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Read Only Data|Anim Graph - Grounded")
	FBMLeanAmount LeanAmount;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Read Only Data|Anim Graph - Grounded")
	float FYaw = 0.0f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Read Only Data|Anim Graph - Grounded")
	float BYaw = 0.0f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Read Only Data|Anim Graph - Grounded")
	float LYaw = 0.0f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Read Only Data|Anim Graph - Grounded")
	float RYaw = 0.0f;

	/** Anim Graph - In Air */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Read Only Data|Anim Graph - In Air")
	bool bJumped = false;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Read Only Data|Anim Graph - In Air")
	float JumpPlayRate = 1.2f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Read Only Data|Anim Graph - In Air")
	float FallSpeed = 0.0f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Read Only Data|Anim Graph - In Air")
	float LandPrediction = 1.0f;

	/** Anim Graph - Aiming Values */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Read Only Data|Anim Graph - Aiming Values")
	FRotator SmoothedAimingRotation;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Read Only Data|Anim Graph - Aiming Values")
	FRotator SpineRotation;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Read Only Data|Anim Graph - Aiming Values")
	FVector2D AimingAngle;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Read Only Data|Anim Graph - Aiming Values")
	FVector2D SmoothedAimingAngle;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Read Only Data|Anim Graph - Aiming Values")
	float AimSweepTime = 0.5f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Read Only Data|Anim Graph - Aiming Values")
	float InputYawOffsetTime = 0.0f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Read Only Data|Anim Graph - Aiming Values")
	float ForwardYawTime = 0.0f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Read Only Data|Anim Graph - Aiming Values")
	float LeftYawTime = 0.0f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Read Only Data|Anim Graph - Aiming Values")
	float RightYawTime = 0.0f;

	/** Anim Graph - Ragdoll */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Read Only Data|Anim Graph - Ragdoll")
	float FlailRate = 0.0f;

	/** Anim Graph - Layer Blending */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Read Only Data|Anim Graph - Layer Blending")
	int32 OverlayOverrideState = 0;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Read Only Data|Anim Graph - Layer Blending")
	float EnableAimOffset = 1.0f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Read Only Data|Anim Graph - Layer Blending")
	float BasePose_N = 1.0f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Read Only Data|Anim Graph - Layer Blending")
	float BasePose_CLF = 0.0f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Read Only Data|Anim Graph - Layer Blending")
	float Arm_L = 0.0f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Read Only Data|Anim Graph - Layer Blending")
	float Arm_L_Add = 0.0f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Read Only Data|Anim Graph - Layer Blending")
	float Arm_L_LS = 0.0f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Read Only Data|Anim Graph - Layer Blending")
	float Arm_L_MS = 0.0f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Read Only Data|Anim Graph - Layer Blending")
	float Arm_R = 0.0f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Read Only Data|Anim Graph - Layer Blending")
	float Arm_R_Add = 0.0f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Read Only Data|Anim Graph - Layer Blending")
	float Arm_R_LS = 0.0f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Read Only Data|Anim Graph - Layer Blending")
	float Arm_R_MS = 0.0f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Read Only Data|Anim Graph - Layer Blending")
	float Hand_L = 0.0f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Read Only Data|Anim Graph - Layer Blending")
	float Hand_R = 0.0f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Read Only Data|Anim Graph - Layer Blending")
	float Legs = 0.0f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Read Only Data|Anim Graph - Layer Blending")
	float Legs_Add = 0.0f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Read Only Data|Anim Graph - Layer Blending")
	float Pelvis = 0.0f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Read Only Data|Anim Graph - Layer Blending")
	float Pelvis_Add = 0.0f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Read Only Data|Anim Graph - Layer Blending")
	float Spine = 0.0f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Read Only Data|Anim Graph - Layer Blending")
	float Spine_Add = 0.0f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Read Only Data|Anim Graph - Layer Blending")
	float Head = 0.0f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Read Only Data|Anim Graph - Layer Blending")
	float Head_Add = 0.0f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Read Only Data|Anim Graph - Layer Blending")
	float EnableHandIK_L = 1.0f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Read Only Data|Anim Graph - Layer Blending")
	float EnableHandIK_R = 1.0f;

	/** Anim Graph - Foot IK */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Read Only Data|Anim Graph - Foot IK")
	float FootLock_L_Alpha = 0.0f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Read Only Data|Anim Graph - Foot IK")
	float FootLock_R_Alpha = 0.0f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Read Only Data|Anim Graph - Foot IK")
	FVector FootLock_L_Location;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Read Only Data|Anim Graph - Foot IK")
	FVector FootLock_R_Location;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Read Only Data|Anim Graph - Foot IK")
	FRotator FootLock_L_Rotation;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Read Only Data|Anim Graph - Foot IK")
	FRotator FootLock_R_Rotation;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Read Only Data|Anim Graph - Foot IK")
	FVector FootOffset_L_Location;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Read Only Data|Anim Graph - Foot IK")
	FVector FootOffset_R_Location;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Read Only Data|Anim Graph - Foot IK")
	FRotator FootOffset_L_Rotation;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Read Only Data|Anim Graph - Foot IK")
	FRotator FootOffset_R_Rotation;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Read Only Data|Anim Graph - Foot IK")
	FVector PelvisOffset;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Read Only Data|Anim Graph - Foot IK")
	float PelvisAlpha = 0.0f;

	/** Turn In Place */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Configuration|Turn In Place")
	float TurnCheckMinAngle = 45.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Configuration|Turn In Place")
	float Turn180Threshold = 130.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Configuration|Turn In Place")
	float AimYawRateLimit = 50.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Configuration|Turn In Place")
	float ElapsedDelayTime = 0.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Configuration|Turn In Place")
	float MinAngleDelay = 0.75f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Configuration|Turn In Place")
	float MaxAngleDelay = 0.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Configuration|Turn In Place")
	FBMTurnInPlaceAsset N_TurnIP_L90;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Configuration|Turn In Place")
	FBMTurnInPlaceAsset N_TurnIP_R90;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Configuration|Turn In Place")
	FBMTurnInPlaceAsset N_TurnIP_L180;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Configuration|Turn In Place")
	FBMTurnInPlaceAsset N_TurnIP_R180;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Configuration|Turn In Place")
	FBMTurnInPlaceAsset CLF_TurnIP_L90;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Configuration|Turn In Place")
	FBMTurnInPlaceAsset CLF_TurnIP_R90;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Configuration|Turn In Place")
	FBMTurnInPlaceAsset CLF_TurnIP_L180;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Configuration|Turn In Place")
	FBMTurnInPlaceAsset CLF_TurnIP_R180;

	/** Rotate In Place */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Configuration|Rotate In Place")
	float RotateMinThreshold = -50.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Configuration|Rotate In Place")
	float RotateMaxThreshold = 50.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Configuration|Rotate In Place")
	float AimYawRateMinRange = 90.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Configuration|Rotate In Place")
	float AimYawRateMaxRange = 270.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Configuration|Rotate In Place")
	float MinPlayRate = 1.15f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Configuration|Rotate In Place")
	float MaxPlayRate = 3.0f;

	/** Configuration */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Configuration|Main Configuration")
	float AnimatedWalkSpeed = 150.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Configuration|Main Configuration")
	float AnimatedRunSpeed = 350.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Configuration|Main Configuration")
	float AnimatedSprintSpeed = 600.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Configuration|Main Configuration")
	float AnimatedCrouchSpeed = 150.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Configuration|Main Configuration")
	float VelocityBlendInterpSpeed = 12.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Configuration|Main Configuration")
	float GroundedLeanInterpSpeed = 4.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Configuration|Main Configuration")
	float InAirLeanInterpSpeed = 4.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Configuration|Main Configuration")
	float SmoothedAimingRotationInterpSpeed = 10.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Configuration|Main Configuration")
	float InputYawOffsetInterpSpeed = 8.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Configuration|Main Configuration")
	float TriggerPivotSpeedLimit = 200.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Configuration|Main Configuration")
	float FootHeight = 13.5f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Configuration|Main Configuration")
	float IK_TraceDistanceAboveFoot = 50.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Configuration|Main Configuration")
	float IK_TraceDistanceBelowFoot = 45.0f;

	/** Blend Curves */

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Configuration|Blend Curves")
	UCurveFloat* DiagonalScaleAmountCurve;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Configuration|Blend Curves")
	UCurveFloat* StrideBlend_N_Walk;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Configuration|Blend Curves")
	UCurveFloat* StrideBlend_N_Run;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Configuration|Blend Curves")
	UCurveFloat* StrideBlend_C_Walk;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Configuration|Blend Curves")
	UCurveFloat* LandPredictionCurve;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Configuration|Blend Curves")
	UCurveFloat* LeanInAirCurve;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Configuration|Blend Curves")
	UCurveVector* YawOffset_FB;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Configuration|Blend Curves")
	UCurveVector* YawOffset_LR;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Configuration|Dynamic Transition")
	UAnimSequenceBase* TransitionAnim_R;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Configuration|Dynamic Transition")
	UAnimSequenceBase* TransitionAnim_L;

private:
	FTimerHandle OnPivotTimer;

	FTimerHandle PlayDynamicTransitionTimer;

	FTimerHandle OnJumpedTimer;

	bool bCanPlayDynamicTransition = true;
};
