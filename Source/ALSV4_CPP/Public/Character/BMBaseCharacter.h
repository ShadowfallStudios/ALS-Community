/** Copyright (c) 2020 Doğa Can YANIKOĞLU */

#pragma once

#include "CoreMinimal.h"
#include "Components/TimelineComponent.h"
#include "Library/BMCharacterEnumLibrary.h"
#include "Library/BMCharacterStructLibrary.h"
#include "Engine/DataTable.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetSystemLibrary.h"


#include "BMBaseCharacter.generated.h"

class UTimelineComponent;
class UAnimInstance;
class UAnimMontage;
class UBMCharacterAnimInstance;

/*
 * Base character class
 */
UCLASS(BlueprintType)
class ALSV4_CPP_API ABMBaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ABMBaseCharacter();

	/** Ragdoll System */

	/** Implement on BP to get required get up animation according to character's state */
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Ragdoll System")
	UAnimMontage* GetGetUpAnimation(bool bRagdollFaceUpState);

	UFUNCTION(BlueprintCallable, Category = "Ragdoll System")
	virtual void RagdollStart();

	UFUNCTION(BlueprintCallable, Category = "Ragdoll System")
	virtual void RagdollEnd();

	/** Character States */

	UFUNCTION(BlueprintCallable, Category = "Character States")
	void SetMovementState(EBMMovementState NewState);

	UFUNCTION(BlueprintGetter, Category = "Character States")
	EBMMovementState GetMovementState() { return MovementState; }

	UFUNCTION(BlueprintGetter, Category = "Character States")
	EBMMovementState GetPrevMovementState() { return PrevMovementState; }

	UFUNCTION(BlueprintCallable, Category = "Character States")
	void SetMovementAction(EBMMovementAction NewAction);

	UFUNCTION(BlueprintGetter, Category = "Character States")
	EBMMovementAction GetMovementAction() { return MovementAction; }

	UFUNCTION(BlueprintCallable, Category = "Character States")
	void SetStance(EBMStance NewStance);

	UFUNCTION(BlueprintGetter, Category = "Character States")
	EBMStance GetStance() { return Stance; }

	UFUNCTION(BlueprintCallable, Category = "Character States")
	void SetRotationMode(EBMRotationMode NewRotationMode);

	UFUNCTION(BlueprintGetter, Category = "Character States")
	EBMRotationMode GetRotationMode() { return RotationMode; }

	UFUNCTION(BlueprintCallable, Category = "Character States")
	void SetGait(EBMGait NewGait);

	UFUNCTION(BlueprintGetter, Category = "Character States")
	EBMGait GetGait() { return Gait; }

	UFUNCTION(BlueprintCallable, Category = "Character States")
	void SetViewMode(EBMViewMode NewViewMode);

	UFUNCTION(BlueprintGetter, Category = "Character States")
	EBMViewMode GetViewMode() { return ViewMode; }

	UFUNCTION(BlueprintCallable, Category = "Character States")
	void SetOverlayState(EBMOverlayState NewState);

	UFUNCTION(BlueprintGetter, Category = "Character States")
	EBMOverlayState GetOverlayState() { return OverlayState; }

	UFUNCTION(BlueprintGetter, Category = "Character States")
	EBMOverlayState SwitchRight() { return OverlayState; }

	/** Input */

	UFUNCTION(BlueprintGetter, Category = "Input")
	EBMStance GetDesiredStance() { return DesiredStance; }

	UFUNCTION(BlueprintSetter, Category = "Input")
	void SetDesiredStance(EBMStance NewStance) { DesiredStance = NewStance; }

	UFUNCTION(BlueprintGetter, Category = "Input")
	EBMGait GetDesiredGait() { return DesiredGait; }

	UFUNCTION(BlueprintSetter, Category = "Input")
	void SetDesiredGait(EBMGait NewGait) { DesiredGait = NewGait; }

	UFUNCTION(BlueprintGetter, Category = "Input")
	EBMRotationMode GetDesiredRotationMode() { return DesiredRotationMode; }

	UFUNCTION(BlueprintSetter, Category = "Input")
	void SetDesiredRotationMode(EBMRotationMode NewRotMode) { DesiredRotationMode = NewRotMode; }

	UFUNCTION(BlueprintCallable, Category = "Input")
	FVector GetPlayerMovementInput();

	/** Rotation System */

	UFUNCTION(BlueprintCallable, Category = "Rotation System")
	void SetActorLocationAndTargetRotation(FVector NewLocation, FRotator NewRotation);

	/** Mantle System */

	/** Implement on BP to get correct mantle parameter set according to character state */
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Mantle System")
	FBMMantleAsset GetMantleAsset(EBMMantleType MantleType);

	UFUNCTION(BlueprintCallable, Category = "Mantle System")
	virtual bool MantleCheckGrounded();

	UFUNCTION(BlueprintCallable, Category = "Mantle System")
	virtual bool MantleCheckFalling();

	/** Movement System */

	UFUNCTION(BlueprintGetter, Category = "Movement System")
	bool HasMovementInput() { return bHasMovementInput; }

	UFUNCTION(BlueprintCallable, Category = "Movement System")
	FBMMovementSettings GetTargetMovementSettings();

	UFUNCTION(BlueprintCallable, Category = "Movement System")
	EBMGait GetAllowedGait();

	UFUNCTION(BlueprintCallable, Category = "Movement States")
	EBMGait GetActualGait(EBMGait AllowedGait);

	UFUNCTION(BlueprintCallable, Category = "Movement System")
	bool CanSprint();

	/** BP implementable function that called when Breakfall starts */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Movement System")
	void OnBreakfall();
	virtual void OnBreakfall_Implementation();

	/** BP implementable function that called when Roll starts */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Movement System")
	void OnRoll();
	virtual void OnRoll_Implementation();

	/** Implement on BP to get required roll animation according to character's state */
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Movement System")
	UAnimMontage* GetRollAnimation();

	/** Utility */

	UFUNCTION(BlueprintCallable, Category = "Utility")
	float GetAnimCurveValue(FName CurveName);

	/** Implement on BP to draw debug spheres */
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Debug")
	void DrawDebugSpheres();

	/** Camera System */

	UFUNCTION(BlueprintGetter, Category = "Camera System")
	bool IsRightShoulder() { return bRightShoulder; }

	UFUNCTION(BlueprintCallable, Category = "Camera System")
	void SetRightShoulder(bool bNewRightShoulder) { bRightShoulder = bNewRightShoulder; }

	UFUNCTION(BlueprintCallable, Category = "Camera System")
	virtual ECollisionChannel GetThirdPersonTraceParams(FVector& TraceOrigin, float& TraceRadius);

	UFUNCTION(BlueprintCallable, Category = "Camera System")
	virtual FTransform GetThirdPersonPivotTarget();

	UFUNCTION(BlueprintCallable, Category = "Camera System")
	virtual FVector GetFirstPersonCameraTarget();

	UFUNCTION(BlueprintCallable, Category = "Camera System")
	void GetCameraParameters(float& TPFOVOut, float& FPFOVOut, bool& bRightShoulderOut);

	/** Essential Information Getters */

	UFUNCTION(BlueprintGetter, Category = "Essential Information")
	FVector GetCurrentAcceleration() { return Acceleration; }

	UFUNCTION(BlueprintGetter, Category = "Essential Information")
	bool IsMoving() { return bIsMoving; }

	UFUNCTION(BlueprintCallable, Category = "Essential Information")
	FVector GetMovementInput();

	UFUNCTION(BlueprintGetter, Category = "Essential Information")
	float GetMovementInputAmount() { return MovementInputAmount; }

	UFUNCTION(BlueprintGetter, Category = "Essential Information")
	float GetSpeed() { return Speed; }

	UFUNCTION(BlueprintCallable, Category = "Essential Information")
	FRotator GetAimingRotation() { return GetControlRotation(); }

	UFUNCTION(BlueprintGetter, Category = "Essential Information")
	float GetAimYawRate() { return AimYawRate; }

protected:
	virtual void Tick(float DeltaTime) override;

	virtual void BeginPlay() override;

	/** Ragdoll System */

	void RagdollUpdate();

	void SetActorLocationDuringRagdoll();

	/** Stace Changes */

	virtual void OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode = 0) override;

	virtual void OnMovementStateChanged(EBMMovementState PreviousState);

	virtual void OnMovementActionChanged(EBMMovementAction PreviousAction);

	virtual void OnStanceChanged(EBMStance PreviousStance);

	virtual void OnRotationModeChanged(EBMRotationMode PreviousRotationMode);

	virtual void OnGaitChanged(EBMGait PreviousGait);

	virtual void OnViewModeChanged(EBMViewMode PreviousViewMode);

	virtual void OnOverlayStateChanged(EBMOverlayState PreviousState);

	void OnStartCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust) override;

	void OnEndCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust) override;

	void OnJumped_Implementation() override;

	void Landed(const FHitResult& Hit) override;

	void OnLandFrictionReset();

	UFUNCTION()
	void OnTimelineUpdated(float BlendIn);

	UFUNCTION()
	void OnTimelineFinished();

	void SetEssentialValues(float DeltaTime);

	void UpdateCharacterMovement();

	void UpdateDynamicMovementSettings(EBMGait AllowedGait);

	void UpdateGroundedRotation(float DeltaTime);

	void UpdateInAirRotation(float DeltaTime);

	/** Mantle System */

	virtual void MantleStart(float MantleHeight, const FBMComponentAndTransform& MantleLedgeWS, EBMMantleType MantleType);

	virtual bool MantleCheck(const FBMMantleTraceSettings& TraceSettings,
	                         EDrawDebugTrace::Type DebugType = EDrawDebugTrace::Type::ForOneFrame);

	virtual void MantleUpdate(float BlendIn);

	virtual void MantleEnd();

	bool CapsuleHasRoomCheck(UCapsuleComponent* Capsule, FVector TargetLocation,
	                         float HeightOffset, float RadiusOffset, EDrawDebugTrace::Type DebugType);

	/** Utils */

	float GetMappedSpeed();

	void SmoothCharacterRotation(FRotator Target, float TargetInterpSpeed, float ActorInterpSpeed, float DeltaTime);

	float CalculateGroundedRotationRate();

	void LimitRotation(float AimYawMin, float AimYawMax, float InterpSpeed, float DeltaTime);

	void SetMovementModel();

protected:
	/** Input */

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Input")
	EBMRotationMode DesiredRotationMode = EBMRotationMode::LookingDirection;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Input")
	EBMGait DesiredGait = EBMGait::Running;

	UPROPERTY(BlueprintReadWrite, Category = "Input")
	EBMStance DesiredStance = EBMStance::Standing;

	/** Camera System */

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Camera System")
	float ThirdPersonFOV = 90.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Camera System")
	float FirstPersonFOV = 90.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Camera System")
	bool bRightShoulder = false;

	/** State Values */

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "State Values")
	EBMOverlayState OverlayState = EBMOverlayState::Default;

	/** Movement System */

	UPROPERTY(BlueprintReadOnly, Category = "Movement System")
	FBMMovementSettings CurrentMovementSettings;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement System")
	FDataTableRowHandle MovementModel;

	/** Mantle System */

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Mantle System")
	FBMMantleTraceSettings GroundedTraceSettings;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Mantle System")
	FBMMantleTraceSettings AutomaticTraceSettings;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Mantle System")
	FBMMantleTraceSettings FallingTraceSettings;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Mantle System")
	UCurveFloat* MantleTimelineCurve;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Misc")
	FName PelvisBoneName = TEXT("Pelvis");

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Misc")
	FName RootBoneName = TEXT("Root");

	UPROPERTY()
	FName RagdollSnapshotName = TEXT("RagdollPose");

	/** Components */

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UTimelineComponent* MantleTimeline = nullptr;

	/** References */

	UPROPERTY()
	UBMCharacterAnimInstance* MainAnimInstance = nullptr;

	/** Essential Information */

	UPROPERTY(BlueprintReadOnly, Category = "Essential Information")
	FVector Acceleration;

	UPROPERTY(BlueprintReadOnly, Category = "Essential Information")
	bool bIsMoving = false;

	UPROPERTY(BlueprintReadOnly, Category = "Essential Information")
	bool bHasMovementInput = false;

	UPROPERTY(BlueprintReadOnly, Category = "Essential Information")
	FRotator LastVelocityRotation;

	UPROPERTY(BlueprintReadOnly, Category = "Essential Information")
	FRotator LastMovementInputRotation;

	UPROPERTY(BlueprintReadOnly, Category = "Essential Information")
	float Speed = 0.0f;

	UPROPERTY(BlueprintReadOnly, Category = "Essential Information")
	float MovementInputAmount = 0.0f;

	UPROPERTY(BlueprintReadOnly, Category = "Essential Information")
	float AimYawRate = 0.0f;

	/** State Values */

	UPROPERTY(BlueprintReadOnly, Category = "State Values")
	EBMMovementState MovementState = EBMMovementState::None;

	UPROPERTY(BlueprintReadOnly, Category = "State Values")
	EBMMovementState PrevMovementState = EBMMovementState::None;

	UPROPERTY(BlueprintReadOnly, Category = "State Values")
	EBMMovementAction MovementAction = EBMMovementAction::None;

	UPROPERTY(BlueprintReadOnly, Category = "State Values")
	EBMRotationMode RotationMode = EBMRotationMode::LookingDirection;

	UPROPERTY(BlueprintReadOnly, Category = "State Values")
	EBMGait Gait = EBMGait::Walking;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "State Values")
	EBMStance Stance = EBMStance::Standing;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "State Values")
	EBMViewMode ViewMode = EBMViewMode::ThirdPerson;

	/** Movement System */

	UPROPERTY(BlueprintReadOnly, Category = "Movement System")
	FBMMovementStateSettings MovementData;

	/** Rotation System */

	UPROPERTY(BlueprintReadOnly, Category = "Rotation System")
	FRotator TargetRotation;

	UPROPERTY(BlueprintReadOnly, Category = "Rotation System")
	FRotator InAirRotation;

	UPROPERTY(BlueprintReadOnly, Category = "Rotation System")
	float YawOffset = 0.0f;

	/** Mantle System */

	UPROPERTY(BlueprintReadOnly, Category = "Mantle System")
	FBMMantleParams MantleParams;

	UPROPERTY(BlueprintReadOnly, Category = "Mantle System")
	FBMComponentAndTransform MantleLedgeLS;

	UPROPERTY(BlueprintReadOnly, Category = "Mantle System")
	FTransform MantleTarget;

	UPROPERTY(BlueprintReadOnly, Category = "Mantle System")
	FTransform MantleActualStartOffset;

	UPROPERTY(BlueprintReadOnly, Category = "Mantle System")
	FTransform MantleAnimatedStartOffset;

	/** Ragdoll System */

	UPROPERTY(BlueprintReadOnly, Category = "Ragdoll System")
	bool bRagdollOnGround = false;

	UPROPERTY(BlueprintReadOnly, Category = "Ragdoll System")
	bool bRagdollFaceUp = false;

	UPROPERTY(BlueprintReadOnly, Category = "Ragdoll System")
	FVector LastRagdollVelocity;

	/** Cached Variables */

	UPROPERTY(BlueprintReadOnly, Category = "Cached Variables")
	FVector PreviousVelocity;

	UPROPERTY(BlueprintReadOnly, Category = "Cached Variables")
	float PreviousAimYaw = 0.0f;

	/* Timer to manage reset of braking friction factor after on landed event */
	FTimerHandle OnLandedFrictionResetTimer;
};
