// Copyright (C) 2020, Doga Can Yanikoglu

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Library/BMAnimationStructLibrary.h"

#include "BMCharacterAnimInstance.generated.h"

class ABMBaseCharacter;
class UCurveFloat;
class UAnimSequence;
class UCurveVector;

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

	UFUNCTION(BlueprintCallable, Category = "Grounded")
	void SetGroundedEntryState(EBMGroundedEntryState GroundedEntryState)
	{
		Grounded.GroundedEntryState = GroundedEntryState;
	}

	UFUNCTION(BlueprintCallable, Category = "Grounded")
	void SetOverlayOverrideState(int32 OverlayOverrideState)
	{
		LayerBlendingValues.OverlayOverrideState = OverlayOverrideState;
	}

	UFUNCTION(BlueprintCallable, Category = "Grounded")
	void SetTrackedHipsDirection(EBMHipsDirection HipsDirection)
	{
		Grounded.TrackedHipsDirection = HipsDirection;
	}

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

	/** Return mutable reference of character information to edit them easily inside character class */
	FBMAnimCharacterInformation& GetCharacterInformationMutable()
	{
		return CharacterInformation;
	}

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

protected:
	/** References */
	UPROPERTY(BlueprintReadOnly)
	ABMBaseCharacter* Character = nullptr;

	/** Character Information */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Read Only Data|Character Information", Meta = (ShowOnlyInnerProperties))
	FBMAnimCharacterInformation CharacterInformation;

	/** Anim Graph - Grounded */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Read Only Data|Anim Graph - Grounded", Meta = (ShowOnlyInnerProperties))
	FBMAnimGraphGrounded Grounded;

	/** Anim Graph - In Air */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Read Only Data|Anim Graph - In Air", Meta = (ShowOnlyInnerProperties))
	FBMAnimGraphInAir InAir;

	/** Anim Graph - Aiming Values */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Read Only Data|Anim Graph - Aiming Values", Meta = (
		ShowOnlyInnerProperties))
	FBMAnimGraphAimingValues AimingValues;

	/** Anim Graph - Ragdoll */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Read Only Data|Anim Graph - Ragdoll")
	float FlailRate = 0.0f;

	/** Anim Graph - Layer Blending */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Read Only Data|Anim Graph - Layer Blending", Meta = (
		ShowOnlyInnerProperties))
	FBMAnimGraphLayerBlending LayerBlendingValues;

	/** Anim Graph - Foot IK */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Read Only Data|Anim Graph - Foot IK", Meta = (ShowOnlyInnerProperties))
	FBMAnimGraphFootIK FootIKValues;

	/** Turn In Place */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Configuration|Turn In Place", Meta = (ShowOnlyInnerProperties))
	FBMAnimTurnInPlace TurnInPlaceValues;

	/** Rotate In Place */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Configuration|Rotate In Place", Meta = (ShowOnlyInnerProperties))
	FBMAnimRotateInPlace RotateInPlace;

	/** Configuration */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Configuration|Main Configuration", Meta = (ShowOnlyInnerProperties))
	FBMAnimConfiguration Config;

	/** Blend Curves */

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Configuration|Blend Curves")
	UCurveFloat* DiagonalScaleAmountCurve;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Configuration|Blend Curves")
	UCurveFloat* StrideBlend_N_Walk;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Configuration|Blend Curves")
	UCurveFloat* StrideBlend_N_Run;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Configuration|Blend Curves")
	UCurveFloat* StrideBlend_C_Walk;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Configuration|Blend Curves")
	UCurveFloat* LandPredictionCurve;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Configuration|Blend Curves")
	UCurveFloat* LeanInAirCurve;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Configuration|Blend Curves")
	UCurveVector* YawOffset_FB;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Configuration|Blend Curves")
	UCurveVector* YawOffset_LR;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Configuration|Dynamic Transition")
	UAnimSequenceBase* TransitionAnim_R;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Configuration|Dynamic Transition")
	UAnimSequenceBase* TransitionAnim_L;

private:
	FTimerHandle OnPivotTimer;

	FTimerHandle PlayDynamicTransitionTimer;

	FTimerHandle OnJumpedTimer;

	bool bCanPlayDynamicTransition = true;
};
