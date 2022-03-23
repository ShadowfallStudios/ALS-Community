// Copyright:       Copyright (C) 2022 Doğa Can Yanıkoğlu
// Source Code:     https://github.com/dyanikoglu/ALS-Community


#include "Components/ALSMantleComponent.h"


#include "Character/ALSCharacter.h"
#include "Character/Animation/ALSCharacterAnimInstance.h"
#include "Components/ALSDebugComponent.h"
#include "Curves/CurveVector.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Library/ALSMathLibrary.h"


const FName NAME_MantleEnd(TEXT("MantleEnd"));
const FName NAME_MantleUpdate(TEXT("MantleUpdate"));
const FName NAME_MantleTimeline(TEXT("MantleTimeline"));

FName UALSMantleComponent::NAME_IgnoreOnlyPawn(TEXT("IgnoreOnlyPawn"));


UALSMantleComponent::UALSMantleComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = true;

	MantleTimeline = CreateDefaultSubobject<UTimelineComponent>(NAME_MantleTimeline);
}

void UALSMantleComponent::BeginPlay()
{
	Super::BeginPlay();

	if (GetOwner())
	{
		OwnerCharacter = Cast<AALSBaseCharacter>(GetOwner());
		if (OwnerCharacter)
		{
			ALSDebugComponent = OwnerCharacter->FindComponentByClass<UALSDebugComponent>();

			AddTickPrerequisiteActor(OwnerCharacter); // Always tick after owner, so we'll use updated values

			// Bindings
			FOnTimelineFloat TimelineUpdated;
			FOnTimelineEvent TimelineFinished;
			TimelineUpdated.BindUFunction(this, NAME_MantleUpdate);
			TimelineFinished.BindUFunction(this, NAME_MantleEnd);
			MantleTimeline->SetTimelineFinishedFunc(TimelineFinished);
			MantleTimeline->SetLooping(false);
			MantleTimeline->SetTimelineLengthMode(TL_TimelineLength);
			MantleTimeline->AddInterpFloat(MantleTimelineCurve, TimelineUpdated);

			OwnerCharacter->JumpPressedDelegate.AddUniqueDynamic(this, &UALSMantleComponent::OnOwnerJumpInput);
			OwnerCharacter->RagdollStateChangedDelegate.AddUniqueDynamic(
				this, &UALSMantleComponent::OnOwnerRagdollStateChanged);
		}
	}
}


void UALSMantleComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                        FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (OwnerCharacter && OwnerCharacter->GetMovementState() == EALSMovementState::InAir)
	{
		// Perform a mantle check if falling while movement input is pressed.
		if (OwnerCharacter->HasMovementInput())
		{
			MantleCheck(FallingTraceSettings, EDrawDebugTrace::Type::ForOneFrame);
		}
	}
}

void UALSMantleComponent::MantleStart(float MantleHeight, const FALSComponentAndTransform& MantleLedgeWS,
                                      EALSMantleType MantleType)
{
	if (OwnerCharacter == nullptr || !IsValid(MantleLedgeWS.Component) || !IsValid(MantleTimeline))
	{
		return;
	}

	if (MantleType != EALSMantleType::LowMantle && OwnerCharacter->IsA(AALSCharacter::StaticClass()))
	{
		Cast<AALSCharacter>(OwnerCharacter)->ClearHeldObject();
	}

	// Disable ticking during mantle
	SetComponentTickEnabledAsync(false);

	// Step 1: Get the Mantle Asset and use it to set the new Mantle Params.
	const FALSMantleAsset MantleAsset = GetMantleAsset(MantleType, OwnerCharacter->GetOverlayState());
	check(MantleAsset.PositionCorrectionCurve)

	MantleParams.AnimMontage = MantleAsset.AnimMontage;
	MantleParams.PositionCorrectionCurve = MantleAsset.PositionCorrectionCurve;
	MantleParams.StartingOffset = MantleAsset.StartingOffset;
	MantleParams.StartingPosition = FMath::GetMappedRangeValueClamped<float, float>({MantleAsset.LowHeight, MantleAsset.HighHeight},
	                                                                  {
		                                                                  MantleAsset.LowStartPosition,
		                                                                  MantleAsset.HighStartPosition
	                                                                  },
	                                                                  MantleHeight);
	MantleParams.PlayRate = FMath::GetMappedRangeValueClamped<float, float>({MantleAsset.LowHeight, MantleAsset.HighHeight},
	                                                          {MantleAsset.LowPlayRate, MantleAsset.HighPlayRate},
	                                                          MantleHeight);

	// Step 2: Convert the world space target to the mantle component's local space for use in moving objects.
	MantleLedgeLS.Component = MantleLedgeWS.Component;
	MantleLedgeLS.Transform = MantleLedgeWS.Transform * MantleLedgeWS.Component->GetComponentToWorld().Inverse();

	// Step 3: Set the Mantle Target and calculate the Starting Offset
	// (offset amount between the actor and target transform).
	MantleTarget = MantleLedgeWS.Transform;
	MantleActualStartOffset = UALSMathLibrary::TransformSub(OwnerCharacter->GetActorTransform(), MantleTarget);

	// Step 4: Calculate the Animated Start Offset from the Target Location.
	// This would be the location the actual animation starts at relative to the Target Transform.
	FVector RotatedVector = MantleTarget.GetRotation().Vector() * MantleParams.StartingOffset.Y;
	RotatedVector.Z = MantleParams.StartingOffset.Z;
	const FTransform StartOffset(MantleTarget.Rotator(), MantleTarget.GetLocation() - RotatedVector,
	                             FVector::OneVector);
	MantleAnimatedStartOffset = UALSMathLibrary::TransformSub(StartOffset, MantleTarget);

	// Step 5: Clear the Character Movement Mode and set the Movement State to Mantling
	OwnerCharacter->GetCharacterMovement()->SetMovementMode(MOVE_None);
	OwnerCharacter->SetMovementState(EALSMovementState::Mantling);

	// Step 6: Configure the Mantle Timeline so that it is the same length as the
	// Lerp/Correction curve minus the starting position, and plays at the same speed as the animation.
	// Then start the timeline.
	float MinTime = 0.0f;
	float MaxTime = 0.0f;
	MantleParams.PositionCorrectionCurve->GetTimeRange(MinTime, MaxTime);
	MantleTimeline->SetTimelineLength(MaxTime - MantleParams.StartingPosition);
	MantleTimeline->SetPlayRate(MantleParams.PlayRate);
	MantleTimeline->PlayFromStart();

	// Step 7: Play the Anim Montage if valid.
	if (MantleParams.AnimMontage && OwnerCharacter->GetMesh()->GetAnimInstance())
	{
		OwnerCharacter->GetMesh()->GetAnimInstance()->Montage_Play(MantleParams.AnimMontage, MantleParams.PlayRate,
		                                                    EMontagePlayReturnType::MontageLength,
		                                                    MantleParams.StartingPosition, false);
	}
}

bool UALSMantleComponent::MantleCheck(const FALSMantleTraceSettings& TraceSettings, EDrawDebugTrace::Type DebugType)
{
	if (!OwnerCharacter)
	{
		return false;
	}

	// Step 1: Trace forward to find a wall / object the character cannot walk on.
	const FVector& TraceDirection = OwnerCharacter->GetActorForwardVector();
	const FVector& CapsuleBaseLocation = UALSMathLibrary::GetCapsuleBaseLocation(
		2.0f, OwnerCharacter->GetCapsuleComponent());
	FVector TraceStart = CapsuleBaseLocation + TraceDirection * -30.0f;
	TraceStart.Z += (TraceSettings.MaxLedgeHeight + TraceSettings.MinLedgeHeight) / 2.0f;
	const FVector TraceEnd = TraceStart + TraceDirection * TraceSettings.ReachDistance;
	const float HalfHeight = 1.0f + (TraceSettings.MaxLedgeHeight - TraceSettings.MinLedgeHeight) / 2.0f;

	UWorld* World = GetWorld();
	check(World);

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(OwnerCharacter);

	FHitResult HitResult;
	{
		const FCollisionShape CapsuleCollisionShape = FCollisionShape::MakeCapsule(TraceSettings.ForwardTraceRadius, HalfHeight);
		const bool bHit = World->SweepSingleByProfile(HitResult, TraceStart, TraceEnd, FQuat::Identity, MantleObjectDetectionProfile,
	                                                  CapsuleCollisionShape, Params);

		if (ALSDebugComponent && ALSDebugComponent->GetShowTraces())
		{
			UALSDebugComponent::DrawDebugCapsuleTraceSingle(World,
			                                                TraceStart,
			                                                TraceEnd,
			                                                CapsuleCollisionShape,
			                                                DebugType,
			                                                bHit,
			                                                HitResult,
			                                                FLinearColor::Black,
			                                                FLinearColor::Black,
			                                                1.0f);
		}
	}

	if (!HitResult.IsValidBlockingHit() || OwnerCharacter->GetCharacterMovement()->IsWalkable(HitResult))
	{
		// Not a valid surface to mantle
		return false;
	}

	if (HitResult.GetComponent() != nullptr)
	{
		UPrimitiveComponent* PrimitiveComponent = HitResult.GetComponent();
		if (PrimitiveComponent && PrimitiveComponent->GetComponentVelocity().Size() > AcceptableVelocityWhileMantling)
		{
			// The surface to mantle moves too fast
			return false;
		}
	}

	const FVector InitialTraceImpactPoint = HitResult.ImpactPoint;
	const FVector InitialTraceNormal = HitResult.ImpactNormal;

	// Step 2: Trace downward from the first trace's Impact Point and determine if the hit location is walkable.
	FVector DownwardTraceEnd = InitialTraceImpactPoint;
	DownwardTraceEnd.Z = CapsuleBaseLocation.Z;
	DownwardTraceEnd += InitialTraceNormal * -15.0f;
	FVector DownwardTraceStart = DownwardTraceEnd;
	DownwardTraceStart.Z += TraceSettings.MaxLedgeHeight + TraceSettings.DownwardTraceRadius + 1.0f;

	{
		const FCollisionShape SphereCollisionShape = FCollisionShape::MakeSphere(TraceSettings.DownwardTraceRadius);
		const bool bHit = World->SweepSingleByChannel(HitResult, DownwardTraceStart, DownwardTraceEnd, FQuat::Identity,
	                                                  WalkableSurfaceDetectionChannel, SphereCollisionShape,
	                                                  Params);

		if (ALSDebugComponent && ALSDebugComponent->GetShowTraces())
		{
			UALSDebugComponent::DrawDebugSphereTraceSingle(World,
			                                               TraceStart,
			                                               TraceEnd,
			                                               SphereCollisionShape,
			                                               DebugType,
			                                               bHit,
			                                               HitResult,
			                                               FLinearColor::Black,
			                                               FLinearColor::Black,
			                                               1.0f);
		}
	}


	if (!OwnerCharacter->GetCharacterMovement()->IsWalkable(HitResult))
	{
		// Not a valid surface to mantle
		return false;
	}

	const FVector DownTraceLocation(HitResult.Location.X, HitResult.Location.Y, HitResult.ImpactPoint.Z);
	UPrimitiveComponent* HitComponent = HitResult.GetComponent();

	// Step 3: Check if the capsule has room to stand at the downward trace's location.
	// If so, set that location as the Target Transform and calculate the mantle height.
	const FVector& CapsuleLocationFBase = UALSMathLibrary::GetCapsuleLocationFromBase(
		DownTraceLocation, 2.0f, OwnerCharacter->GetCapsuleComponent());
	const bool bCapsuleHasRoom = UALSMathLibrary::CapsuleHasRoomCheck(OwnerCharacter->GetCapsuleComponent(),
	                                                                  CapsuleLocationFBase, 0.0f,
	                                                                  0.0f, DebugType, ALSDebugComponent && ALSDebugComponent->GetShowTraces());

	if (!bCapsuleHasRoom)
	{
		// Capsule doesn't have enough room to mantle
		return false;
	}

	const FTransform TargetTransform(
		(InitialTraceNormal * FVector(-1.0f, -1.0f, 0.0f)).ToOrientationRotator(),
		CapsuleLocationFBase,
		FVector::OneVector);

	const float MantleHeight = (CapsuleLocationFBase - OwnerCharacter->GetActorLocation()).Z;

	// Step 4: Determine the Mantle Type by checking the movement mode and Mantle Height.
	EALSMantleType MantleType;
	if (OwnerCharacter->GetMovementState() == EALSMovementState::InAir)
	{
		MantleType = EALSMantleType::FallingCatch;
	}
	else
	{
		MantleType = MantleHeight > 125.0f ? EALSMantleType::HighMantle : EALSMantleType::LowMantle;
	}

	// Step 5: If everything checks out, start the Mantle
	FALSComponentAndTransform MantleWS;
	MantleWS.Component = HitComponent;
	MantleWS.Transform = TargetTransform;
	MantleStart(MantleHeight, MantleWS, MantleType);
	Server_MantleStart(MantleHeight, MantleWS, MantleType);

	return true;
}

void UALSMantleComponent::Server_MantleStart_Implementation(float MantleHeight,
                                                            const FALSComponentAndTransform& MantleLedgeWS,
                                                            EALSMantleType MantleType)
{
	Multicast_MantleStart(MantleHeight, MantleLedgeWS, MantleType);
}

void UALSMantleComponent::Multicast_MantleStart_Implementation(float MantleHeight,
                                                               const FALSComponentAndTransform& MantleLedgeWS,
                                                               EALSMantleType MantleType)
{
	if (OwnerCharacter && !OwnerCharacter->IsLocallyControlled())
	{
		MantleStart(MantleHeight, MantleLedgeWS, MantleType);
	}
}

// This function is called by "MantleTimeline" using BindUFunction in UALSMantleComponent::BeginPlay during the default settings initialization.
void UALSMantleComponent::MantleUpdate(float BlendIn)
{
	if (!OwnerCharacter)
	{
		return;
	}

	// Step 1: Continually update the mantle target from the stored local transform to follow along with moving objects
	MantleTarget = UALSMathLibrary::MantleComponentLocalToWorld(MantleLedgeLS);

	// Step 2: Update the Position and Correction Alphas using the Position/Correction curve set for each Mantle.
	const FVector CurveVec = MantleParams.PositionCorrectionCurve
	                                     ->GetVectorValue(
		                                     MantleParams.StartingPosition + MantleTimeline->GetPlaybackPosition());
	const float PositionAlpha = CurveVec.X;
	const float XYCorrectionAlpha = CurveVec.Y;
	const float ZCorrectionAlpha = CurveVec.Z;

	// Step 3: Lerp multiple transforms together for independent control over the horizontal
	// and vertical blend to the animated start position, as well as the target position.

	// Blend into the animated horizontal and rotation offset using the Y value of the Position/Correction Curve.
	const FTransform TargetHzTransform(MantleAnimatedStartOffset.GetRotation(),
	                                   {
		                                   MantleAnimatedStartOffset.GetLocation().X,
		                                   MantleAnimatedStartOffset.GetLocation().Y,
		                                   MantleActualStartOffset.GetLocation().Z
	                                   },
	                                   FVector::OneVector);
	const FTransform& HzLerpResult =
		UKismetMathLibrary::TLerp(MantleActualStartOffset, TargetHzTransform, XYCorrectionAlpha);

	// Blend into the animated vertical offset using the Z value of the Position/Correction Curve.
	const FTransform TargetVtTransform(MantleActualStartOffset.GetRotation(),
	                                   {
		                                   MantleActualStartOffset.GetLocation().X,
		                                   MantleActualStartOffset.GetLocation().Y,
		                                   MantleAnimatedStartOffset.GetLocation().Z
	                                   },
	                                   FVector::OneVector);
	const FTransform& VtLerpResult =
		UKismetMathLibrary::TLerp(MantleActualStartOffset, TargetVtTransform, ZCorrectionAlpha);

	const FTransform ResultTransform(HzLerpResult.GetRotation(),
	                                 {
		                                 HzLerpResult.GetLocation().X, HzLerpResult.GetLocation().Y,
		                                 VtLerpResult.GetLocation().Z
	                                 },
	                                 FVector::OneVector);

	// Blend from the currently blending transforms into the final mantle target using the X
	// value of the Position/Correction Curve.
	const FTransform& ResultLerp = UKismetMathLibrary::TLerp(
		UALSMathLibrary::TransformAdd(MantleTarget, ResultTransform), MantleTarget,
		PositionAlpha);

	// Initial Blend In (controlled in the timeline curve) to allow the actor to blend into the Position/Correction
	// curve at the midpoint. This prevents pops when mantling an object lower than the animated mantle.
	const FTransform& LerpedTarget =
		UKismetMathLibrary::TLerp(UALSMathLibrary::TransformAdd(MantleTarget, MantleActualStartOffset), ResultLerp,
		                          BlendIn);

	// Step 4: Set the actors location and rotation to the Lerped Target.
	OwnerCharacter->SetActorLocationAndTargetRotation(LerpedTarget.GetLocation(), LerpedTarget.GetRotation().Rotator());
}

void UALSMantleComponent::MantleEnd()
{
	// Set the Character Movement Mode to Walking
	if (OwnerCharacter)
	{
		OwnerCharacter->GetCharacterMovement()->SetMovementMode(MOVE_Walking);

		if (OwnerCharacter->IsA(AALSCharacter::StaticClass()))
		{
			Cast<AALSCharacter>(OwnerCharacter)->UpdateHeldObject();
		}
	}

	// Enable ticking back after mantle ends
	SetComponentTickEnabledAsync(true);
}

void UALSMantleComponent::OnOwnerJumpInput()
{
	// Check if character is able to do one of the special mantling

	if (OwnerCharacter && OwnerCharacter->GetMovementAction() == EALSMovementAction::None)
	{
		if (OwnerCharacter->GetMovementState() == EALSMovementState::Grounded)
		{
			if (OwnerCharacter->HasMovementInput())
			{
				MantleCheck(GroundedTraceSettings, EDrawDebugTrace::Type::ForDuration);
			}
		}
		else if (OwnerCharacter->GetMovementState() == EALSMovementState::InAir)
		{
			MantleCheck(FallingTraceSettings, EDrawDebugTrace::Type::ForDuration);
		}
	}
}

void UALSMantleComponent::OnOwnerRagdollStateChanged(bool bRagdollState)
{
	// If owner is going into ragdoll state, stop mantling immediately
	if (bRagdollState)
	{
		MantleTimeline->Stop();
	}
}
