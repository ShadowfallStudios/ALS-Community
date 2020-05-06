// Copyright (C) 2020, Doga Can Yanikoglu

#pragma once

#include "CoreMinimal.h"
#include "BMCharacterEnumLibrary.generated.h"

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


UENUM(BlueprintType)
enum class EBMGait : uint8
{
	Walking,
	Running,
	Sprinting
};

UENUM(BlueprintType)
enum class EBMMovementAction : uint8
{
	None,
	LowMantle,
	HighMantle,
	Rolling,
	GettingUp
};

UENUM(BlueprintType)
enum class EBMMovementState : uint8
{
	None,
	Grounded,
	InAir,
	Mantling,
	Ragdoll
};

UENUM(BlueprintType)
enum class EBMOverlayState : uint8
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

UENUM(BlueprintType)
enum class EBMRotationMode : uint8
{
	VelocityDirection,
	LookingDirection,
	Aiming
};

UENUM(BlueprintType)
enum class EBMStance : uint8
{
	Standing,
	Crouching
};

UENUM(BlueprintType)
enum class EBMViewMode : uint8
{
	ThirdPerson,
	FirstPerson
};

UENUM(BlueprintType)
enum class EBMAnimFeatureExample : uint8
{
	StrideBlending,
	AdditiveBlending,
	SprintImpulse
};

UENUM(BlueprintType)
enum class EBMFootstepType : uint8
{
	Step,
	WalkRun,
	Jump,
	Land
};

UENUM(BlueprintType)
enum class EBMGroundedEntryState : uint8
{
	None,
	Roll
};

UENUM(BlueprintType)
enum class EBMHipsDirection : uint8
{
	F,
	B,
	RF,
	RB,
	LF,
	LB
};

UENUM(BlueprintType)
enum class EBMMantleType : uint8
{
	HighMantle,
	LowMantle,
	FallingCatch
};

UENUM(BlueprintType)
enum class EBMMovementDirection : uint8
{
	Forward,
	Right,
	Left,
	Backward
};

#if WITH_EDITOR
static TMap<int32, FString> OverlayOverrideStateMap
{
	TPair<int32, FString>(0, "Default"),
	TPair<int32, FString>(1, "Masculine"),
	TPair<int32, FString>(2, "Feminine"),
	TPair<int32, FString>(3, "Injured"),
	TPair<int32, FString>(4, "Hands Tied"),
	TPair<int32, FString>(5, "Rifle"),
	TPair<int32, FString>(6, "Pistol 1H"),
	TPair<int32, FString>(7, "Pistol 2H"),
	TPair<int32, FString>(8, "Bow"),
	TPair<int32, FString>(9, "Torch"),
	TPair<int32, FString>(10, "Binoculars"),
	TPair<int32, FString>(11, "Box"),
	TPair<int32, FString>(12, "Barrel")
};
#endif
