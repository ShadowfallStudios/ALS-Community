// Copyright:       Copyright (C) 2022 Doğa Can Yanıkoğlu
// Source Code:     https://github.com/dyanikoglu/ALS-Community


#pragma once

#include "CoreMinimal.h"
#include "ALSCharacterEnumLibrary.generated.h"

/* Returns the enumeration index. */
template <typename Enumeration>
static FORCEINLINE int32 GetEnumerationIndex(const Enumeration InValue)
{
	return StaticEnum<Enumeration>()->GetIndexByValue(static_cast<int64>(InValue));
}

/* Returns the enumeration value as string. */
template <typename Enumeration>
static FORCEINLINE FString GetEnumerationToString(const Enumeration InValue)
{
	return StaticEnum<Enumeration>()->GetNameStringByValue(static_cast<int64>(InValue));
}

/**
 * Character gait state. Note: Also edit related struct in ALSStructEnumLibrary if you add new enums
 */
UENUM(BlueprintType)
enum class EALSGait : uint8
{
	Walking,
	Running,
	Sprinting
};

/**
 * Character movement action state. Note: Also edit related struct in ALSStructEnumLibrary if you add new enums
 */
UENUM(BlueprintType)
enum class EALSMovementAction : uint8
{
	None,
	LowMantle,
	HighMantle,
	Rolling,
	GettingUp
};

/**
 * Character movement state. Note: Also edit related struct in ALSStructEnumLibrary if you add new enums
 */
UENUM(BlueprintType)
enum class EALSMovementState : uint8
{
	None,
	Grounded,
	InAir,
	Mantling,
	Ragdoll
};

/**
 * Character overlay state. Note: Also edit related struct in ALSStructEnumLibrary if you add new enums
 */
UENUM(BlueprintType)
enum class EALSOverlayState : uint8
{
	Default,
	Masculine,
	Feminine,
	Injured,
	HandsTied,
	Rifle,
	PistolOneHanded,
	PistolTwoHanded,
	Bow,
	Torch,
	Binoculars,
	Box,
	Barrel
};

/**
 * Character rotation mode. Note: Also edit related struct in ALSStructEnumLibrary if you add new enums
 */
UENUM(BlueprintType)
enum class EALSRotationMode : uint8
{
	VelocityDirection,
	LookingDirection,
	Aiming
};

/**
 * Character stance. Note: Also edit related struct in ALSStructEnumLibrary if you add new enums
 */
UENUM(BlueprintType)
enum class EALSStance : uint8
{
	Standing,
	Crouching
};

/**
 * Character view mode. Note: Also edit related struct in ALSStructEnumLibrary if you add new enums
 */
UENUM(BlueprintType)
enum class EALSViewMode : uint8
{
	ThirdPerson,
	FirstPerson
};

UENUM(BlueprintType)
enum class EALSAnimFeatureExample : uint8
{
	StrideBlending,
	AdditiveBlending,
	SprintImpulse
};

UENUM(BlueprintType)
enum class EALSFootstepType : uint8
{
	Step,
	WalkRun,
	Jump,
	Land
};

UENUM(BlueprintType)
enum class EALSGroundedEntryState : uint8
{
	None,
	Roll
};

UENUM(BlueprintType)
enum class EALSHipsDirection : uint8
{
	F,
	B,
	RF,
	RB,
	LF,
	LB
};

UENUM(BlueprintType)
enum class EALSMantleType : uint8
{
	HighMantle,
	LowMantle,
	FallingCatch
};

UENUM(BlueprintType)
enum class EALSMovementDirection : uint8
{
	Forward,
	Right,
	Left,
	Backward
};

UENUM(BlueprintType)
enum class EALSSpawnType : uint8
{
	Location,
	Attached
};
