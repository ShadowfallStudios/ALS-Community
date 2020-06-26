// Copyright (C) 2020, Doga Can Yanikoglu

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

	virtual void Tick(float DeltaTime) override;

	virtual void BeginPlay() override;

	virtual void PreInitializeComponents() override;

	virtual void Restart() override;

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

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
	void SetGait(EBMGait NewGait);

	UFUNCTION(BlueprintGetter, Category = "Character States")
	EBMGait GetGait() { return Gait; }

	UFUNCTION(BlueprintCallable, Category = "Character States")
	void SetDesiredGait(EBMGait NewGait);

	UFUNCTION(BlueprintCallable, Server, Reliable, Category = "Character States")
	void Server_SetDesiredGait(EBMGait NewGait);

	UFUNCTION(BlueprintCallable, NetMulticast, Reliable, Category = "Character States")
	void Multicast_SetDesiredGait(EBMGait NewGait);

	UFUNCTION(BlueprintGetter, Category = "CharacterStates")
	EBMGait GetDesiredGait() { return DesiredGait; }

	UFUNCTION(BlueprintCallable, Category = "Character States")
	void SetRotationMode(EBMRotationMode NewRotationMode);

	UFUNCTION(BlueprintCallable, Server, Reliable, Category = "Character States")
	void Server_SetRotationMode(EBMRotationMode NewRotationMode);

	UFUNCTION(BlueprintCallable, NetMulticast, Reliable, Category = "Character States")
	void Multicast_SetRotationMode(EBMRotationMode NewRotationMode);

	UFUNCTION(BlueprintGetter, Category = "Character States")
	EBMRotationMode GetRotationMode() { return RotationMode; }

	UFUNCTION(BlueprintCallable, Category = "Character States")
	void SetViewMode(EBMViewMode NewViewMode);

	UFUNCTION(BlueprintCallable, Server, Reliable, Category = "Character States")
	void Server_SetViewMode(EBMViewMode NewViewMode);

	UFUNCTION(BlueprintCallable, NetMulticast, Reliable, Category = "Character States")
	void Multicast_SetViewMode(EBMViewMode NewViewMode);

	UFUNCTION(BlueprintGetter, Category = "Character States")
	EBMViewMode GetViewMode() { return ViewMode; }

	UFUNCTION(BlueprintCallable, Category = "Character States")
	void SetOverlayState(EBMOverlayState NewState);

	UFUNCTION(BlueprintCallable, Server, Reliable, Category = "Character States")
	void Server_SetOverlayState(EBMOverlayState NewState);

	UFUNCTION(BlueprintCallable, NetMulticast, Reliable, Category = "Character States")
	void Multicast_SetOverlayState(EBMOverlayState NewState);

	UFUNCTION(BlueprintGetter, Category = "Character States")
	EBMOverlayState GetOverlayState() { return OverlayState; }

	UFUNCTION(BlueprintGetter, Category = "Character States")
	EBMOverlayState SwitchRight() { return OverlayState; }

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
	void Server_MantleStart(float MantleHeight, const FBMComponentAndTransform& MantleLedgeWS, EBMMantleType MantleType);

	UFUNCTION(BlueprintCallable, NetMulticast, Reliable, Category = "Character States")
	void Multicast_MantleStart(float MantleHeight, const FBMComponentAndTransform& MantleLedgeWS, EBMMantleType MantleType);

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

	UFUNCTION(BlueprintCallable, Server, Reliable, Category = "Character States")
	void Server_RagdollUpdate(FVector CharacterLocation, FRotator CharacterRotation);

	/** Input */

	UFUNCTION(BlueprintGetter, Category = "Input")
	EBMStance GetDesiredStance() { return DesiredStance; }

	UFUNCTION(BlueprintSetter, Category = "Input")
	void SetDesiredStance(EBMStance NewStance) { DesiredStance = NewStance; }

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
	void SetHasMovementInput(bool bNewHasMovementInput);

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

	/** BP implementable function that called when A Montage starts, e.g. during rolling */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Movement System")
	void Replicated_PlayMontage(UAnimMontage* montage, float track);
	virtual void Replicated_PlayMontage_Implementation(UAnimMontage* montage, float track);

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

	/** Essential Information Getters/Setters */

	UFUNCTION(BlueprintGetter, Category = "Essential Information")
	FVector GetAcceleration() { return Acceleration; }

	UFUNCTION(BlueprintCallable, Category = "Essential Information")
	void SetAcceleration(const FVector& NewAcceleration);

	UFUNCTION(BlueprintCallable, Server, Unreliable, Category = "EssentialInformation")
	void Server_SetAcceleration(const FVector& NewAcceleration);

	UFUNCTION(BlueprintGetter, Category = "Essential Information")
	bool IsMoving() { return bIsMoving; }

	UFUNCTION(BlueprintCallable, Category = "Essential Information")
	void SetIsMoving(bool bNewIsMoving);

	UFUNCTION(BlueprintCallable, Category = "Essential Information")
	FVector GetMovementInput();

	UFUNCTION(BlueprintGetter, Category = "Essential Information")
	float GetMovementInputAmount() { return MovementInputAmount; }

	UFUNCTION(BlueprintCallable, Category = "Essential Information")
	void SetMovementInputAmount(float NewMovementInputAmount);

	UFUNCTION(BlueprintGetter, Category = "Essential Information")
	float GetSpeed() { return Speed; }

	UFUNCTION(BlueprintCallable, Category = "Essential Information")
	void SetSpeed(float NewSpeed);

	UFUNCTION(BlueprintCallable)
	FRotator GetAimingRotation() { return CustomControlRotation; }

	UFUNCTION(BlueprintGetter, Category = "Essential Information")
	float GetAimYawRate() { return AimYawRate; }

	UFUNCTION(BlueprintCallable, Category = "Essential Information")
	void SetAimYawRate(float NewAimYawRate);

	UFUNCTION(BlueprintCallable, Category = "Essential Information")
	void GetControlForwardRightVector(FVector& Forward, FVector& Right);

protected:
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

	void SetEssentialValues(float DeltaTime);

	void UpdateCharacterMovement();

	void UpdateDynamicMovementSettings(EBMGait AllowedGait);

	void UpdateGroundedRotation(float DeltaTime);

	void UpdateInAirRotation(float DeltaTime);

	/** Mantle System */

	virtual void MantleStart(float MantleHeight, const FBMComponentAndTransform& MantleLedgeWS, EBMMantleType MantleType);

	virtual bool MantleCheck(const FBMMantleTraceSettings& TraceSettings,
	                         EDrawDebugTrace::Type DebugType = EDrawDebugTrace::Type::ForOneFrame);

	UFUNCTION()
	virtual void MantleUpdate(float BlendIn);
	
	UFUNCTION()
	virtual void MantleEnd();

	bool CapsuleHasRoomCheck(UCapsuleComponent* Capsule, FVector TargetLocation,
	                         float HeightOffset, float RadiusOffset, EDrawDebugTrace::Type DebugType);

	/** Utils */

	float GetMappedSpeed();

	void SmoothCharacterRotation(FRotator Target, float TargetInterpSpeed, float ActorInterpSpeed, float DeltaTime);

	float CalculateGroundedRotationRate();

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

protected:
	/** Input */

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Input")
	EBMRotationMode DesiredRotationMode = EBMRotationMode::LookingDirection;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Input")
	EBMGait DesiredGait = EBMGait::Running;

	UPROPERTY(BlueprintReadWrite, Category = "Input")
	EBMStance DesiredStance = EBMStance::Standing;

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

	UPROPERTY(BlueprintReadOnly, replicated, Category = "Essential Information")
	FRotator CustomControlRotation;

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

	UPROPERTY(BlueprintReadOnly, replicated, Category = "Rotation System")
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
	bool bReplicateRagdollLocation = false;

	UPROPERTY(BlueprintReadOnly, Category = "Ragdoll System")
	FVector LastRagdollVelocity;

	UPROPERTY(BlueprintReadOnly, Category = "Ragdoll System")
	FVector TargetRagdollLocation;

	/** Cached Variables */

	UPROPERTY(BlueprintReadOnly)
	FVector PreviousVelocity;

	float PreviousAimYaw = 0.0f;

	UPROPERTY(BlueprintReadOnly)
	UBMCharacterAnimInstance* MainAnimInstance = nullptr;

	/** Replication */
	UPROPERTY(BlueprintReadOnly, replicated, Category = "Replication")
	bool OnDedicatedServer;

	/** Last time the 'first' crouch/roll button is pressed */
	float LastStanceInputTime = 0.0f;

	/** Last time the camera action button is pressed */
	float CameraActionPressedTime = 0.0f;

	/* Timer to manage camera mode swap action */
	FTimerHandle OnCameraModeSwapTimer;

	/* Timer to manage reset of braking friction factor after on landed event */
	FTimerHandle OnLandedFrictionResetTimer;
};
