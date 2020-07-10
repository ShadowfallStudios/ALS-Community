// Copyright (C) 2020, Doga Can Yanikoglu


#include "Library/ALSMathLibrary.h"

#include "Components/CapsuleComponent.h"
#include "Library/ALSCharacterStructLibrary.h"

FTransform UALSMathLibrary::MantleComponentLocalToWorld(const FALSComponentAndTransform& CompAndTransform)
{
	const FTransform& InverseTransform = CompAndTransform.Component->GetComponentToWorld().Inverse();
	const FVector Location = InverseTransform.InverseTransformPosition(CompAndTransform.Transform.GetLocation());
	const FQuat Quat = InverseTransform.InverseTransformRotation(CompAndTransform.Transform.GetRotation());
	const FVector Scale = InverseTransform.InverseTransformPosition(CompAndTransform.Transform.GetScale3D());
	return {Quat, Location, Scale};
}

TPair<float, float> UALSMathLibrary::FixDiagonalGamepadValues(const float Y, const float X)
{
	float ResultY = Y * FMath::GetMappedRangeValueClamped(FVector2D(0.0f, 0.6f),
	                                                      FVector2D(1.0f, 1.2f), FMath::Abs(X));
	ResultY = FMath::Clamp(ResultY, -1.0f, 1.0f);
	float ResultX = X * FMath::GetMappedRangeValueClamped(FVector2D(0.0f, 0.6f),
	                                                      FVector2D(1.0f, 1.2f), FMath::Abs(Y));
	ResultX = FMath::Clamp(ResultX, -1.0f, 1.0f);
	return TPair<float, float>(ResultY, ResultX);
}

FVector UALSMathLibrary::GetCapsuleBaseLocation(const float ZOffset, UCapsuleComponent* Capsule)
{
	return Capsule->GetComponentLocation() -
		Capsule->GetUpVector() * (Capsule->GetScaledCapsuleHalfHeight() + ZOffset);
}

FVector UALSMathLibrary::GetCapsuleLocationFromBase(FVector BaseLocation, const float ZOffset,
                                                    UCapsuleComponent* Capsule)
{
	BaseLocation.Z += Capsule->GetScaledCapsuleHalfHeight() + ZOffset;
	return BaseLocation;
}

bool UALSMathLibrary::CapsuleHasRoomCheck(UCapsuleComponent* Capsule, FVector TargetLocation, float HeightOffset,
                                          float RadiusOffset)
{
	// Perform a trace to see if the capsule has room to be at the target location.
	const float ZTarget = Capsule->GetScaledCapsuleHalfHeight_WithoutHemisphere() - RadiusOffset + HeightOffset;
	FVector TraceStart = TargetLocation;
	TraceStart.Z += ZTarget;
	FVector TraceEnd = TargetLocation;
	TraceEnd.Z -= ZTarget;
	const float Radius = Capsule->GetUnscaledCapsuleRadius() + RadiusOffset;

	const UWorld* World = Capsule->GetWorld();
	check(World);

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(Capsule->GetOwner());

	FHitResult HitResult;
	World->SweepSingleByProfile(HitResult, TraceStart, TraceEnd, FQuat::Identity,
	                            FName(TEXT("ALS_Character")), FCollisionShape::MakeSphere(Radius), Params);

	return !(HitResult.bBlockingHit || HitResult.bStartPenetrating);
}
