/** Copyright (c) 2020 Doğa Can YANIKOĞLU */

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Library/BMCharacterEnumLibrary.h"

#include "BMCharacterStructLibrary.generated.h"

class UCurveVector;
class UAnimMontage;
class UAnimSequenceBase;
class UCurveFloat;

USTRUCT(BlueprintType)
struct FBMComponentAndTransform
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FTransform Transform;

	UPROPERTY(EditAnywhere)
	class UPrimitiveComponent* Component = nullptr;
};

USTRUCT(BlueprintType)
struct FBMCameraSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	float TargetArmLength = 0.0f;

	UPROPERTY(EditAnywhere)
	FVector SocketOffset;

	UPROPERTY(EditAnywhere)
	float LagSpeed = 0.0f;

	UPROPERTY(EditAnywhere)
	float RotationLagSpeed = 0.0f;

	UPROPERTY(EditAnywhere)
	bool bDoCollisionTest = true;
};

USTRUCT(BlueprintType)
struct FBMCameraGaitSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FBMCameraSettings Walking;

	UPROPERTY(EditAnywhere)
	FBMCameraSettings Running;

	UPROPERTY(EditAnywhere)
	FBMCameraSettings Sprinting;

	UPROPERTY(EditAnywhere)
	FBMCameraSettings Crouching;
};

USTRUCT(BlueprintType)
struct FBMCameraStateSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FBMCameraGaitSettings VelocityDirection;

	UPROPERTY(EditAnywhere)
	FBMCameraGaitSettings LookingDirection;

	UPROPERTY(EditAnywhere)
	FBMCameraGaitSettings Aiming;
};

USTRUCT(BlueprintType)
struct FBMMantleAsset
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	UAnimMontage* AnimMontage = nullptr;

	UPROPERTY(EditAnywhere)
	UCurveVector* PositionCorrectionCurve = nullptr;

	UPROPERTY(EditAnywhere)
	FVector StartingOffset;

	UPROPERTY(EditAnywhere)
	float LowHeight = 0.0f;

	UPROPERTY(EditAnywhere)
	float LowPlayRate = 0.0f;

	UPROPERTY(EditAnywhere)
	float LowStartPosition = 0.0f;

	UPROPERTY(EditAnywhere)
	float HighHeight = 0.0f;

	UPROPERTY(EditAnywhere)
	float HighPlayRate = 0.0f;

	UPROPERTY(EditAnywhere)
	float HighStartPosition = 0.0f;
};

USTRUCT(BlueprintType)
struct FBMMantleParams
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	UAnimMontage* AnimMontage = nullptr;

	UPROPERTY(EditAnywhere)
	UCurveVector* PositionCorrectionCurve = nullptr;

	UPROPERTY(EditAnywhere)
	float StartingPosition = 0.0f;

	UPROPERTY(EditAnywhere)
	float PlayRate = 0.0f;

	UPROPERTY(EditAnywhere)
	FVector StartingOffset;
};

USTRUCT(BlueprintType)
struct FBMMantleTraceSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	float MaxLedgeHeight = 0.0f;

	UPROPERTY(EditAnywhere)
	float MinLedgeHeight = 0.0f;

	UPROPERTY(EditAnywhere)
	float ReachDistance = 0.0f;

	UPROPERTY(EditAnywhere)
	float ForwardTraceRadius = 0.0f;

	UPROPERTY(EditAnywhere)
	float DownwardTraceRadius = 0.0f;
};

USTRUCT(BlueprintType)
struct FBMMovementSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	float WalkSpeed = 0.0f;

	UPROPERTY(EditAnywhere)
	float RunSpeed = 0.0f;

	UPROPERTY(EditAnywhere)
	float SprintSpeed = 0.0f;

	UPROPERTY(EditAnywhere)
	UCurveVector* MovementCurve = nullptr;

	UPROPERTY(EditAnywhere)
	UCurveFloat* RotationRateCurve = nullptr;

	float GetSpeedForGait(EBMGait Gait)
	{
		switch (Gait)
		{
		case EBMGait::Running:
			return RunSpeed;
		case EBMGait::Sprinting:
			return SprintSpeed;
		case EBMGait::Walking:
			return WalkSpeed;
		default:
			return RunSpeed;
		}
	}
};

USTRUCT(BlueprintType)
struct FBMMovementStanceSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FBMMovementSettings Standing;

	UPROPERTY(EditAnywhere)
	FBMMovementSettings Crouching;
};

USTRUCT(BlueprintType)
struct FBMMovementStateSettings : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FBMMovementStanceSettings VelocityDirection;

	UPROPERTY(EditAnywhere)
	FBMMovementStanceSettings LookingDirection;

	UPROPERTY(EditAnywhere)
	FBMMovementStanceSettings Aiming;
};

USTRUCT(BlueprintType)
struct FBMRotateInPlaceAsset
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	UAnimSequenceBase* Animation = nullptr;

	UPROPERTY(EditAnywhere)
	FName SlotName;

	UPROPERTY(EditAnywhere)
	float SlowTurnRate = 90.0f;

	UPROPERTY(EditAnywhere)
	float FastTurnRate = 90.0f;

	UPROPERTY(EditAnywhere)
	float SlowPlayRate = 1.0f;

	UPROPERTY(EditAnywhere)
	float FastPlayRate = 1.0f;
};
