// Project:         Advanced Locomotion System V4 on C++
// Copyright:       Copyright (C) 2020 Doğa Can Yanıkoğlu
// License:         MIT License (http://www.opensource.org/licenses/mit-license.php)
// Source Code:     https://github.com/dyanikoglu/ALSV4_CPP
// Original Author: Doğa Can Yanıkoğlu
// Contributors:    senfkorn92


#pragma once

#include "CoreMinimal.h"
#include "Components/TimelineComponent.h"
#include "Library/ALSCharacterEnumLibrary.h"
#include "Library/ALSCharacterStructLibrary.h"
#include "Engine/DataTable.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetSystemLibrary.h"

#include "ALSBaseCharacter.generated.h"

class UTimelineComponent;
class UAnimInstance;
class UAnimMontage;
class UALSCharacterAnimInstance;

/*
 * Base character class
 */
UCLASS(BlueprintType)
class ALSV4_CPP_API AALSBaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AALSBaseCharacter(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable, Category= "Movement")
		FORCEINLINE class UALSCharacterMovementComponent* GetMyMovementComponent() const { return MyCharacterMovementComponent; }

	virtual void Tick(float DeltaTime) override;

	virtual void BeginPlay() override;

	virtual void PreInitializeComponents() override;

	virtual void Restart() override;

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	virtual void PostInitializeComponents() override;

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

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
	void SetMovementState(EALSMovementState NewState);

	UFUNCTION(BlueprintGetter, Category = "Character States")
	EALSMovementState GetMovementState() const { return MovementState; }

	UFUNCTION(BlueprintGetter, Category = "Character States")
	EALSMovementState GetPrevMovementState() const { return PrevMovementState; }

	UFUNCTION(BlueprintCallable, Category = "Character States")
	void SetMovementAction(EALSMovementAction NewAction);

	UFUNCTION(BlueprintGetter, Category = "Character States")
	EALSMovementAction GetMovementAction() const { return MovementAction; }

	UFUNCTION(BlueprintCallable, Category = "Character States")
	void SetStance(EALSStance NewStance);

	UFUNCTION(BlueprintGetter, Category = "Character States")
	EALSStance GetStance() const { return Stance; }

	UFUNCTION(BlueprintCallable, Category = "Character States")
	void SetGait(EALSGait NewGait);

	UFUNCTION(BlueprintGetter, Category = "Character States")
	EALSGait GetGait() const { return Gait; }

	UFUNCTION(BlueprintGetter, Category = "CharacterStates")
	EALSGait GetDesiredGait() const { return DesiredGait; }

	UFUNCTION(BlueprintCallable, Category = "Character States")
	void SetRotationMode(EALSRotationMode NewRotationMode);

	UFUNCTION(BlueprintCallable, Server, Reliable, Category = "Character States")
	void Server_SetRotationMode(EALSRotationMode NewRotationMode);

	UFUNCTION(BlueprintGetter, Category = "Character States")
	EALSRotationMode GetRotationMode() const { return RotationMode; }

	UFUNCTION(BlueprintCallable, Category = "Character States")
	void SetViewMode(EALSViewMode NewViewMode);

	UFUNCTION(BlueprintCallable, Server, Reliable, Category = "Character States")
	void Server_SetViewMode(EALSViewMode NewViewMode);

	UFUNCTION(BlueprintGetter, Category = "Character States")
	EALSViewMode GetViewMode() const { return ViewMode; }

	UFUNCTION(BlueprintCallable, Category = "Character States")
	void SetOverlayState(EALSOverlayState NewState);

	UFUNCTION(BlueprintCallable, Server, Reliable, Category = "Character States")
	void Server_SetOverlayState(EALSOverlayState NewState);

	UFUNCTION(BlueprintGetter, Category = "Character States")
	EALSOverlayState GetOverlayState() const { return OverlayState; }

	UFUNCTION(BlueprintGetter, Category = "Character States")
	EALSOverlayState SwitchRight() const { return OverlayState; }

	/** Landed, Jumped, Rolling, Mantling and Ragdoll*/
	/** On Landed*/
	UFUNCTION(BlueprintCallable, Category = "Character States")
	void EventOnLanded();

	UFUNCTION(BlueprintCallable, NetMulticast, Reliable, Category = "Character States")
	void Multicast_OnLanded();

	/** On Jumped*/
	UFUNCTION(BlueprintCallable, Category = "Character States")
	void EventOnJumped();

	UFUNCTION(BlueprintCallable, NetMulticast, Reliable, Category = "Character States")
	void Multicast_OnJumped();

	/** Rolling Montage Play Replication*/
	UFUNCTION(BlueprintCallable, Server, Reliable, Category = "Character States")
	void Server_PlayMontage(UAnimMontage* montage, float track);

	UFUNCTION(BlueprintCallable, NetMulticast, Reliable, Category = "Character States")
	void Multicast_PlayMontage(UAnimMontage* montage, float track);

	/** Mantling*/
	UFUNCTION(BlueprintCallable, Server, Reliable, Category = "Character States")
	void Server_MantleStart(float MantleHeight, const FALSComponentAndTransform& MantleLedgeWS, EALSMantleType MantleType);

	UFUNCTION(BlueprintCallable, NetMulticast, Reliable, Category = "Character States")
	void Multicast_MantleStart(float MantleHeight, const FALSComponentAndTransform& MantleLedgeWS, EALSMantleType MantleType);

	/** Ragolling*/
	UFUNCTION(BlueprintCallable, Category = "Character States")
	void ReplicatedRagdollStart();

	UFUNCTION(BlueprintCallable, Server, Reliable, Category = "Character States")
	void Server_RagdollStart();

	UFUNCTION(BlueprintCallable, NetMulticast, Reliable, Category = "Character States")
	void Multicast_RagdollStart();

	UFUNCTION(BlueprintCallable, Category = "Character States")
	void ReplicatedRagdollEnd();

	UFUNCTION(BlueprintCallable, Server, Reliable, Category = "Character States")
	void Server_RagdollEnd(FVector CharacterLocation);

	UFUNCTION(BlueprintCallable, NetMulticast, Reliable, Category = "Character States")
	void Multicast_RagdollEnd(FVector CharacterLocation);

	/** Input */

	UFUNCTION(BlueprintGetter, Category = "Input")
	EALSStance GetDesiredStance() const { return DesiredStance; }

	UFUNCTION(BlueprintSetter, Category = "Input")
	void SetDesiredStance(EALSStance NewStance);

	UFUNCTION(BlueprintCallable, Server, Reliable, Category = "Input")
	void Server_SetDesiredStance(EALSStance NewStance);

	UFUNCTION(BlueprintCallable, Category = "Character States")
	void SetDesiredGait(EALSGait NewGait);

	UFUNCTION(BlueprintCallable, Server, Reliable, Category = "Character States")
	void Server_SetDesiredGait(EALSGait NewGait);
	
	UFUNCTION(BlueprintGetter, Category = "Input")
	EALSRotationMode GetDesiredRotationMode() const { return DesiredRotationMode; }

	UFUNCTION(BlueprintSetter, Category = "Input")
	void SetDesiredRotationMode(EALSRotationMode NewRotMode);

	UFUNCTION(BlueprintCallable, Server, Reliable, Category = "Character States")
	void Server_SetDesiredRotationMode(EALSRotationMode NewRotMode);

	UFUNCTION(BlueprintCallable, Category = "Input")
	FVector GetPlayerMovementInput() const;

	/** Rotation System */

	UFUNCTION(BlueprintCallable, Category = "Rotation System")
	void SetActorLocationAndTargetRotation(FVector NewLocation, FRotator NewRotation);

	/** Mantle System */

	/** Implement on BP to get correct mantle parameter set according to character state */
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Mantle System")
	FALSMantleAsset GetMantleAsset(EALSMantleType MantleType);

	UFUNCTION(BlueprintCallable, Category = "Mantle System")
	virtual bool MantleCheckGrounded();

	UFUNCTION(BlueprintCallable, Category = "Mantle System")
	virtual bool MantleCheckFalling();

	/** Movement System */

	UFUNCTION(BlueprintGetter, Category = "Movement System")
	bool HasMovementInput() const { return bHasMovementInput; }

	UFUNCTION(BlueprintCallable, Category = "Movement System")
	void SetHasMovementInput(bool bNewHasMovementInput);

	UFUNCTION(BlueprintCallable, Category = "Movement System")
	FALSMovementSettings GetTargetMovementSettings() const;

	UFUNCTION(BlueprintCallable, Category = "Movement System")
	EALSGait GetAllowedGait() const;

	UFUNCTION(BlueprintCallable, Category = "Movement States")
	EALSGait GetActualGait(EALSGait AllowedGait) const;

	UFUNCTION(BlueprintCallable, Category = "Movement System")
	bool CanSprint() const;

	/** BP implementable function that called when Breakfall starts */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Movement System")
	void OnBreakfall();
	virtual void OnBreakfall_Implementation();

	/** BP implementable function that called when A Montage starts, e.g. during rolling */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Movement System")
	void Replicated_PlayMontage(UAnimMontage* montage, float track);
	virtual void Replicated_PlayMontage_Implementation(UAnimMontage* montage, float track);

	/** Implement on BP to get required roll animation according to character's state */
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Movement System")
	UAnimMontage* GetRollAnimation();

	/** Utility */

	UFUNCTION(BlueprintCallable, Category = "Utility")
	float GetAnimCurveValue(FName CurveName) const;

	/** Implement on BP to draw debug spheres */
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Debug")
	void DrawDebugSpheres();

	/** Camera System */

	UFUNCTION(BlueprintGetter, Category = "Camera System")
	bool IsRightShoulder() const { return bRightShoulder; }

	UFUNCTION(BlueprintCallable, Category = "Camera System")
	void SetRightShoulder(bool bNewRightShoulder) { bRightShoulder = bNewRightShoulder; }

	UFUNCTION(BlueprintCallable, Category = "Camera System")
	virtual ECollisionChannel GetThirdPersonTraceParams(FVector& TraceOrigin, float& TraceRadius);

	UFUNCTION(BlueprintCallable, Category = "Camera System")
	virtual FTransform GetThirdPersonPivotTarget();

	UFUNCTION(BlueprintCallable, Category = "Camera System")
	virtual FVector GetFirstPersonCameraTarget();

	UFUNCTION(BlueprintCallable, Category = "Camera System")
	void GetCameraParameters(float& TPFOVOut, float& FPFOVOut, bool& bRightShoulderOut) const;

	/** Essential Information Getters/Setters */

	UFUNCTION(BlueprintGetter, Category = "Essential Information")
	FVector GetAcceleration() const { return Acceleration; }

	UFUNCTION(BlueprintCallable, Category = "Essential Information")
	void SetAcceleration(const FVector& NewAcceleration);

	UFUNCTION(BlueprintGetter, Category = "Essential Information")
	bool IsMoving() const { return bIsMoving; }

	UFUNCTION(BlueprintCallable, Category = "Essential Information")
	void SetIsMoving(bool bNewIsMoving);

	UFUNCTION(BlueprintCallable, Category = "Essential Information")
	FVector GetMovementInput() const;

	UFUNCTION(BlueprintGetter, Category = "Essential Information")
	float GetMovementInputAmount() const { return MovementInputAmount; }

	UFUNCTION(BlueprintCallable, Category = "Essential Information")
	void SetMovementInputAmount(float NewMovementInputAmount);

	UFUNCTION(BlueprintGetter, Category = "Essential Information")
	float GetSpeed() const { return Speed; }

	UFUNCTION(BlueprintCallable, Category = "Essential Information")
	void SetSpeed(float NewSpeed);

	UFUNCTION(BlueprintCallable)
	FRotator GetAimingRotation() const { return AimingRotation; }

	UFUNCTION(BlueprintGetter, Category = "Essential Information")
	float GetAimYawRate() const { return AimYawRate; }

	UFUNCTION(BlueprintCallable, Category = "Essential Information")
	void SetAimYawRate(float NewAimYawRate);

	UFUNCTION(BlueprintCallable, Category = "Essential Information")
	void GetControlForwardRightVector(FVector& Forward, FVector& Right) const;

protected:
	/** Ragdoll System */

	void RagdollUpdate(float DeltaTime);

	void SetActorLocationDuringRagdoll(float DeltaTime);

	/** Stace Changes */

	virtual void OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode = 0) override;

	virtual void OnMovementStateChanged(EALSMovementState PreviousState);

	virtual void OnMovementActionChanged(EALSMovementAction PreviousAction);

	virtual void OnStanceChanged(EALSStance PreviousStance);

	virtual void OnRotationModeChanged(EALSRotationMode PreviousRotationMode);

	virtual void OnGaitChanged(EALSGait PreviousGait);

	virtual void OnViewModeChanged(EALSViewMode PreviousViewMode);

	virtual void OnOverlayStateChanged(EALSOverlayState PreviousState);

	void OnStartCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust) override;

	void OnEndCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust) override;

	void OnJumped_Implementation() override;

	void Landed(const FHitResult& Hit) override;

	void OnLandFrictionReset();

	void SetEssentialValues(float DeltaTime);

	void UpdateCharacterMovement();

	void UpdateDynamicMovementSettings(EALSGait AllowedGait);

	void UpdateGroundedRotation(float DeltaTime);

	void UpdateInAirRotation(float DeltaTime);

	/** Mantle System */

	virtual void MantleStart(float MantleHeight, const FALSComponentAndTransform& MantleLedgeWS, EALSMantleType MantleType);

	virtual bool MantleCheck(const FALSMantleTraceSettings& TraceSettings,
		EDrawDebugTrace::Type DebugType = EDrawDebugTrace::Type::ForOneFrame);

	UFUNCTION()
	virtual void MantleUpdate(float BlendIn);

	UFUNCTION()
	virtual void MantleEnd();

	/** Utils */

	float GetMappedSpeed() const;

	void SmoothCharacterRotation(FRotator Target, float TargetInterpSpeed, float ActorInterpSpeed, float DeltaTime);

	float CalculateGroundedRotationRate() const;

	void LimitRotation(float AimYawMin, float AimYawMax, float InterpSpeed, float DeltaTime);

	void SetMovementModel();

	/** Input */

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

	/** Replication */
	UFUNCTION()
	void OnRep_RotationMode(EALSRotationMode PrevRotMode);

	UFUNCTION()
	void OnRep_ViewMode(EALSViewMode PrevViewMode);

	UFUNCTION()
	void OnRep_OverlayState(EALSOverlayState PrevOverlayState);

protected:
	/* Custom movement component*/
	UALSCharacterMovementComponent* MyCharacterMovementComponent;

	/** Input */

	UPROPERTY(EditAnywhere, replicated, BlueprintReadWrite, Category = "Input")
	EALSRotationMode DesiredRotationMode = EALSRotationMode::LookingDirection;

	UPROPERTY(EditAnywhere, replicated, BlueprintReadWrite, Category = "Input")
	EALSGait DesiredGait = EALSGait::Running;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, replicated, Category = "Input")
	EALSStance DesiredStance = EALSStance::Standing;

	UPROPERTY(EditDefaultsOnly, Category = "Input", BlueprintReadOnly)
	float LookUpDownRate = 1.25f;

	UPROPERTY(EditDefaultsOnly, Category = "Input", BlueprintReadOnly)
	float LookLeftRightRate = 1.25f;

	UPROPERTY(EditDefaultsOnly, Category = "Input", BlueprintReadOnly)
	float RollDoubleTapTimeout = 0.3f;

	UPROPERTY(EditDefaultsOnly, Category = "Input", BlueprintReadOnly)
	float ViewModeSwitchHoldTime = 0.2f;

	UPROPERTY(Category = "Input", BlueprintReadOnly)
	int32 TimesPressedStance = 0;

	UPROPERTY(Category = "Input", BlueprintReadOnly)
	bool bBreakFall = false;

	UPROPERTY(Category = "Input", BlueprintReadOnly)
	bool bSprintHeld = false;

	/** Camera System */

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Camera System")
	float ThirdPersonFOV = 90.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Camera System")
	float FirstPersonFOV = 90.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Camera System")
	bool bRightShoulder = false;

	/** State Values */

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "State Values", ReplicatedUsing = OnRep_OverlayState)
	EALSOverlayState OverlayState = EALSOverlayState::Default;

	/** Movement System */

	UPROPERTY(BlueprintReadOnly, Category = "Movement System")
	FALSMovementSettings CurrentMovementSettings;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement System")
	FDataTableRowHandle MovementModel;

	/** Mantle System */

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Mantle System")
	FALSMantleTraceSettings GroundedTraceSettings;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Mantle System")
	FALSMantleTraceSettings AutomaticTraceSettings;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Mantle System")
	FALSMantleTraceSettings FallingTraceSettings;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Mantle System")
	UCurveFloat* MantleTimelineCurve;

	/** Components */

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UTimelineComponent* MantleTimeline = nullptr;

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

	UPROPERTY(BlueprintReadOnly, replicated, Category = "Essential Information")
	float MovementInputAmount = 0.0f;

	UPROPERTY(BlueprintReadOnly, Category = "Essential Information")
	float AimYawRate = 0.0f;

	/** Replicated Essential Information*/

	UPROPERTY(BlueprintReadOnly, Category = "Essential Information")
	float EasedMaxAcceleration;

	UPROPERTY(BlueprintReadOnly, replicated, Category = "Essential Information")
	FVector ReplicatedCurrentAcceleration;

	UPROPERTY(BlueprintReadOnly, replicated, Category = "Essential Information")
	FRotator ReplicatedControlRotation;

	/** State Values */

	UPROPERTY(BlueprintReadOnly, Category = "State Values")
	EALSMovementState MovementState = EALSMovementState::None;

	UPROPERTY(BlueprintReadOnly, Category = "State Values")
	EALSMovementState PrevMovementState = EALSMovementState::None;

	UPROPERTY(BlueprintReadOnly, Category = "State Values")
	EALSMovementAction MovementAction = EALSMovementAction::None;

	UPROPERTY(BlueprintReadOnly, Category = "State Values", ReplicatedUsing = OnRep_RotationMode)
	EALSRotationMode RotationMode = EALSRotationMode::LookingDirection;

	UPROPERTY(BlueprintReadOnly, Category = "State Values")
	EALSGait Gait = EALSGait::Walking;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "State Values")
	EALSStance Stance = EALSStance::Standing;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "State Values", ReplicatedUsing = OnRep_ViewMode)
	EALSViewMode ViewMode = EALSViewMode::ThirdPerson;

	/** Movement System */

	UPROPERTY(BlueprintReadOnly, Category = "Movement System")
	FALSMovementStateSettings MovementData;

	/** Rotation System */

	UPROPERTY(BlueprintReadOnly, Category = "Rotation System")
	FRotator TargetRotation;

	UPROPERTY(BlueprintReadOnly, Category = "Rotation System")
	FRotator InAirRotation;

	UPROPERTY(BlueprintReadOnly, Category = "Rotation System")
	float YawOffset = 0.0f;

	/** Mantle System */

	UPROPERTY(BlueprintReadOnly, Category = "Mantle System")
	FALSMantleParams MantleParams;

	UPROPERTY(BlueprintReadOnly, Category = "Mantle System")
	FALSComponentAndTransform MantleLedgeLS;

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

	UPROPERTY(BlueprintReadOnly, replicated, Category = "Ragdoll System")
	FVector TargetRagdollLocation;

	/* Server ragdoll pull force storage*/
	float ServerRagdollPull = 0.0f;

	/* Dedicated server mesh default visibility based anim tick option*/
	EVisibilityBasedAnimTickOption DefVisBasedTickOp = EVisibilityBasedAnimTickOption::AlwaysTickPoseAndRefreshBones;

	/** Cached Variables */

	FVector PreviousVelocity;

	float PreviousAimYaw = 0.0f;

	UPROPERTY(BlueprintReadOnly)
	UALSCharacterAnimInstance* MainAnimInstance = nullptr;

	/** Last time the 'first' crouch/roll button is pressed */
	float LastStanceInputTime = 0.0f;

	/** Last time the camera action button is pressed */
	float CameraActionPressedTime = 0.0f;

	/* Timer to manage camera mode swap action */
	FTimerHandle OnCameraModeSwapTimer;

	/* Timer to manage reset of braking friction factor after on landed event */
	FTimerHandle OnLandedFrictionResetTimer;

	/* Smooth out aiming by interping control rotation*/
	FRotator AimingRotation;

	/* Default interp speed for client-server synchronisation*/
	float SynchroInterpSpeed = 20;
};
