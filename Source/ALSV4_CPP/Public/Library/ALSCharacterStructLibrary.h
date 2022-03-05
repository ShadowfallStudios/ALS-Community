// Copyright:       Copyright (C) 2022 Doğa Can Yanıkoğlu
// Source Code:     https://github.com/dyanikoglu/ALS-Community


#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Materials/MaterialInterface.h"
#include "Library/ALSCharacterEnumLibrary.h"

#include "ALSCharacterStructLibrary.generated.h"

class UCurveVector;
class UAnimMontage;
class UAnimSequenceBase;
class UCurveFloat;
class UNiagaraSystem;
class UMaterialInterface;
class USoundBase;
class UPrimitiveComponent;

USTRUCT(BlueprintType)
struct FALSComponentAndTransform
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Character Struct Library")
	FTransform Transform;

	UPROPERTY(EditAnywhere, Category = "Character Struct Library")
	TObjectPtr<UPrimitiveComponent> Component = nullptr;
};

USTRUCT(BlueprintType)
struct FALSCameraSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Camera")
	float TargetArmLength = 0.0f;

	UPROPERTY(EditAnywhere, Category = "Camera")
	FVector SocketOffset;

	UPROPERTY(EditAnywhere, Category = "Camera")
	float LagSpeed = 0.0f;

	UPROPERTY(EditAnywhere, Category = "Camera")
	float RotationLagSpeed = 0.0f;

	UPROPERTY(EditAnywhere, Category = "Camera")
	bool bDoCollisionTest = true;
};

USTRUCT(BlueprintType)
struct FALSCameraGaitSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Camera")
	FALSCameraSettings Walking;

	UPROPERTY(EditAnywhere, Category = "Camera")
	FALSCameraSettings Running;

	UPROPERTY(EditAnywhere, Category = "Camera")
	FALSCameraSettings Sprinting;

	UPROPERTY(EditAnywhere, Category = "Camera")
	FALSCameraSettings Crouching;
};

USTRUCT(BlueprintType)
struct FALSCameraStateSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Camera")
	FALSCameraGaitSettings VelocityDirection;

	UPROPERTY(EditAnywhere, Category = "Camera")
	FALSCameraGaitSettings LookingDirection;

	UPROPERTY(EditAnywhere, Category = "Camera")
	FALSCameraGaitSettings Aiming;
};

USTRUCT(BlueprintType)
struct FALSMantleAsset
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Mantle System")
	TObjectPtr<UAnimMontage> AnimMontage = nullptr;

	UPROPERTY(EditAnywhere, Category = "Mantle System")
	TObjectPtr<UCurveVector> PositionCorrectionCurve = nullptr;

	UPROPERTY(EditAnywhere, Category = "Mantle System")
	FVector StartingOffset;

	UPROPERTY(EditAnywhere, Category = "Mantle System")
	float LowHeight = 0.0f;

	UPROPERTY(EditAnywhere, Category = "Mantle System")
	float LowPlayRate = 0.0f;

	UPROPERTY(EditAnywhere, Category = "Mantle System")
	float LowStartPosition = 0.0f;

	UPROPERTY(EditAnywhere, Category = "Mantle System")
	float HighHeight = 0.0f;

	UPROPERTY(EditAnywhere, Category = "Mantle System")
	float HighPlayRate = 0.0f;

	UPROPERTY(EditAnywhere, Category = "Mantle System")
	float HighStartPosition = 0.0f;
};

USTRUCT(BlueprintType)
struct FALSMantleParams
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Mantle System")
	TObjectPtr<UAnimMontage> AnimMontage = nullptr;

	UPROPERTY(EditAnywhere, Category = "Mantle System")
	TObjectPtr<UCurveVector> PositionCorrectionCurve = nullptr;

	UPROPERTY(EditAnywhere, Category = "Mantle System")
	float StartingPosition = 0.0f;

	UPROPERTY(EditAnywhere, Category = "Mantle System")
	float PlayRate = 0.0f;

	UPROPERTY(EditAnywhere, Category = "Mantle System")
	FVector StartingOffset;
};

USTRUCT(BlueprintType)
struct FALSMantleTraceSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Mantle System")
	float MaxLedgeHeight = 0.0f;

	UPROPERTY(EditAnywhere, Category = "Mantle System")
	float MinLedgeHeight = 0.0f;

	UPROPERTY(EditAnywhere, Category = "Mantle System")
	float ReachDistance = 0.0f;

	UPROPERTY(EditAnywhere, Category = "Mantle System")
	float ForwardTraceRadius = 0.0f;

	UPROPERTY(EditAnywhere, Category = "Mantle System")
	float DownwardTraceRadius = 0.0f;
};

USTRUCT(BlueprintType)
struct FALSMovementSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Movement Settings")
	float WalkSpeed = 0.0f;

	UPROPERTY(EditAnywhere, Category = "Movement Settings")
	float RunSpeed = 0.0f;

	UPROPERTY(EditAnywhere, Category = "Movement Settings")
	float SprintSpeed = 0.0f;

	UPROPERTY(EditAnywhere, Category = "Movement Settings")
	TObjectPtr<UCurveVector> MovementCurve = nullptr;

	UPROPERTY(EditAnywhere, Category = "Movement Settings")
	TObjectPtr<UCurveFloat> RotationRateCurve = nullptr;

	float GetSpeedForGait(const EALSGait Gait) const
	{
		switch (Gait)
		{
		case EALSGait::Running:
			return RunSpeed;
		case EALSGait::Sprinting:
			return SprintSpeed;
		case EALSGait::Walking:
			return WalkSpeed;
		default:
			return RunSpeed;
		}
	}
};

USTRUCT(BlueprintType)
struct FALSMovementStanceSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Movement Settings")
	FALSMovementSettings Standing;

	UPROPERTY(EditAnywhere, Category = "Movement Settings")
	FALSMovementSettings Crouching;
};

USTRUCT(BlueprintType)
struct FALSMovementStateSettings : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Movement Settings")
	FALSMovementStanceSettings VelocityDirection;

	UPROPERTY(EditAnywhere, Category = "Movement Settings")
	FALSMovementStanceSettings LookingDirection;

	UPROPERTY(EditAnywhere, Category = "Movement Settings")
	FALSMovementStanceSettings Aiming;
};

USTRUCT(BlueprintType)
struct FALSRotateInPlaceAsset
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Rotation System")
	TObjectPtr<UAnimSequenceBase> Animation = nullptr;

	UPROPERTY(EditAnywhere, Category = "Rotation System")
	FName SlotName;

	UPROPERTY(EditAnywhere, Category = "Rotation System")
	float SlowTurnRate = 90.0f;

	UPROPERTY(EditAnywhere, Category = "Rotation System")
	float FastTurnRate = 90.0f;

	UPROPERTY(EditAnywhere, Category = "Rotation System")
	float SlowPlayRate = 1.0f;

	UPROPERTY(EditAnywhere, Category = "Rotation System")
	float FastPlayRate = 1.0f;
};

USTRUCT(BlueprintType)
struct FALSHitFX : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Surface")
	TEnumAsByte<enum EPhysicalSurface> SurfaceType;

	UPROPERTY(EditAnywhere, Category = "Sound")
	TSoftObjectPtr<USoundBase> Sound;

	UPROPERTY(EditAnywhere, Category = "Sound")
	EALSSpawnType SoundSpawnType;

	UPROPERTY(EditAnywhere, Category = "Sound", meta = (EditCondition = "SoundSpawnType == EALSSpawnType::Attached"))
	TEnumAsByte<enum EAttachLocation::Type> SoundAttachmentType;

	UPROPERTY(EditAnywhere, Category = "Sound")
	FVector SoundLocationOffset;

	UPROPERTY(EditAnywhere, Category = "Sound")
	FRotator SoundRotationOffset;

	UPROPERTY(EditAnywhere, Category = "Decal")
	TSoftObjectPtr<UMaterialInterface> DecalMaterial;

	UPROPERTY(EditAnywhere, Category = "Decal")
	EALSSpawnType DecalSpawnType;

	UPROPERTY(EditAnywhere, Category = "Decal", meta = (EditCondition = "DecalSpawnType == EALSSpawnType::Attached"))
	TEnumAsByte<enum EAttachLocation::Type> DecalAttachmentType;

	UPROPERTY(EditAnywhere, Category = "Decal")
	float DecalLifeSpan = 10.0f;

	UPROPERTY(EditAnywhere, Category = "Decal")
	FVector DecalSize;

	UPROPERTY(EditAnywhere, Category = "Decal")
	FVector DecalLocationOffset;

	UPROPERTY(EditAnywhere, Category = "Decal")
	FRotator DecalRotationOffset;

	UPROPERTY(EditAnywhere, Category = "Niagara")
	TSoftObjectPtr<UNiagaraSystem> NiagaraSystem;

	UPROPERTY(EditAnywhere, Category = "Niagara")
	EALSSpawnType NiagaraSpawnType;

	UPROPERTY(EditAnywhere, Category = "Niagara", meta = (EditCondition = "NiagaraSpawnType == EALSSpawnType::Attached"))
	TEnumAsByte<enum EAttachLocation::Type> NiagaraAttachmentType;

	UPROPERTY(EditAnywhere, Category = "Niagara")
	FVector NiagaraLocationOffset;

	UPROPERTY(EditAnywhere, Category = "Niagara")
	FRotator NiagaraRotationOffset;
};
