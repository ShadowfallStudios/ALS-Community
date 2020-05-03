/** Copyright (c) 2020 Doğa Can YANIKOĞLU */


#include "Character/BMPlayerCameraManager.h"

#include "Character/BMBaseCharacter.h"
#include "Character/Animation/BMPlayerCameraBehavior.h"
#include "Kismet/KismetMathLibrary.h"

ABMPlayerCameraManager::ABMPlayerCameraManager()
{
	CameraBehavior = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CameraBehavior"));
	CameraBehavior->SetupAttachment(GetRootComponent());
	CameraBehavior->bHiddenInGame = true;
}

void ABMPlayerCameraManager::OnPossess(ABMBaseCharacter* NewCharacter)
{
	// Set "Controlled Pawn" when Player Controller Possesses new character. (called from Player Controller)
	check(NewCharacter);
	ControlledCharacter = NewCharacter;

	// Update references in the Camera Behavior AnimBP.
	UBMPlayerCameraBehavior* CastedBehv = Cast<UBMPlayerCameraBehavior>(CameraBehavior->GetAnimInstance());
	if (CastedBehv)
	{
		CastedBehv->PlayerController = GetOwningPlayerController();
		CastedBehv->ControlledPawn = ControlledCharacter;
	}
}

float ABMPlayerCameraManager::GetCameraBehaviorParam(FName CurveName)
{
	UAnimInstance* Inst = CameraBehavior->GetAnimInstance();
	if (Inst)
	{
		return Inst->GetCurveValue(CurveName);
	}
	return 0.0f;
}

void ABMPlayerCameraManager::UpdateViewTargetInternal(FTViewTarget& OutVT, float DeltaTime)
{
	// Partially taken from base class

	if (OutVT.Target)
	{
		FVector OutLocation;
		FRotator OutRotation;
		float OutFOV;

		if (OutVT.Target->ActorHasTag(CustomTag) && CustomCameraBehavior(DeltaTime, OutLocation, OutRotation, OutFOV))
		{
			OutVT.POV.Location = OutLocation;
			OutVT.POV.Rotation = OutRotation;
			OutVT.POV.FOV = OutFOV;
		}
		else
		{
			OutVT.Target->CalcCamera(DeltaTime, OutVT.POV);
		}
	}
}

FVector ABMPlayerCameraManager::CalculateAxisIndependentLag(FVector CurrentLocation, FVector TargetLocation,
                                                            FRotator CameraRotation, FVector LagSpeeds, float DeltaTime)
{
	CameraRotation.Roll = 0.0f;
	CameraRotation.Pitch = 0.0f;
	const FVector UnrotatedCurLoc = CameraRotation.UnrotateVector(CurrentLocation);
	const FVector UnrotatedTargetLoc = CameraRotation.UnrotateVector(TargetLocation);

	const FVector ResultVector(
		FMath::FInterpTo(UnrotatedCurLoc.X, UnrotatedTargetLoc.X, DeltaTime, LagSpeeds.X),
		FMath::FInterpTo(UnrotatedCurLoc.Y, UnrotatedTargetLoc.Y, DeltaTime, LagSpeeds.Y),
		FMath::FInterpTo(UnrotatedCurLoc.Z, UnrotatedTargetLoc.Z, DeltaTime, LagSpeeds.Z));

	return CameraRotation.RotateVector(ResultVector);
}

bool ABMPlayerCameraManager::CustomCameraBehavior(float DeltaTime, FVector& Location, FRotator& Rotation, float& FOV)
{
	// Step 1: Get Camera Parameters from CharacterBP via the Camera Interface
	const FTransform& PivotTarget = ControlledCharacter->GetThirdPersonPivotTarget();
	const FVector& FPTarget = ControlledCharacter->GetFirstPersonCameraTarget();
	float TPFOV = 90.0f;
	float FPFOV = 90.0f;
	bool bRightShoulder = false;
	ControlledCharacter->GetCameraParameters(TPFOV, FPFOV, bRightShoulder);

	// Step 2: Calculate Target Camera Rotation. Use the Control Rotation and interpolate for smooth camera rotation.
	const FRotator& InterpResult = FMath::RInterpTo(GetCameraRotation(),
	                                                GetOwningPlayerController()->GetControlRotation(), DeltaTime,
	                                                GetCameraBehaviorParam(TEXT("RotationLagSpeed")));

	TargetCameraRotation = UKismetMathLibrary::RLerp(InterpResult, DebugViewRotation,
	                                                 GetCameraBehaviorParam(TEXT("Override_Debug")), true);

	// Step 3: Calculate the Smoothed Pivot Target (Orange Sphere).
	// Get the 3P Pivot Target (Green Sphere) and interpolate using axis independent lag for maximum control.
	const FVector LagSpd(GetCameraBehaviorParam(TEXT("PivotLagSpeed_X")),
	                     GetCameraBehaviorParam(TEXT("PivotLagSpeed_Y")),
	                     GetCameraBehaviorParam(TEXT("PivotLagSpeed_Z")));

	const FVector& AxisIndpLag = CalculateAxisIndependentLag(SmoothedPivotTarget.GetLocation(),
	                                                         PivotTarget.GetLocation(), TargetCameraRotation, LagSpd, DeltaTime);

	SmoothedPivotTarget.SetRotation(PivotTarget.GetRotation());
	SmoothedPivotTarget.SetLocation(AxisIndpLag);
	SmoothedPivotTarget.SetScale3D(FVector::OneVector);

	// Step 4: Calculate Pivot Location (BlueSphere). Get the Smoothed
	// Pivot Target and apply local offsets for further camera control.
	PivotLocation =
		SmoothedPivotTarget.GetLocation() +
		UKismetMathLibrary::GetForwardVector(SmoothedPivotTarget.Rotator()) * GetCameraBehaviorParam(TEXT("PivotOffset_X")) +
		UKismetMathLibrary::GetRightVector(SmoothedPivotTarget.Rotator()) * GetCameraBehaviorParam(TEXT("PivotOffset_Y")) +
		UKismetMathLibrary::GetUpVector(SmoothedPivotTarget.Rotator()) * GetCameraBehaviorParam(TEXT("PivotOffset_Z"));

	// Step 5: Calculate Target Camera Location. Get the Pivot location and apply camera relative offsets.
	TargetCameraLocation = UKismetMathLibrary::VLerp(
		PivotLocation +
		UKismetMathLibrary::GetForwardVector(TargetCameraRotation) * GetCameraBehaviorParam(TEXT("CameraOffset_X")) +
		UKismetMathLibrary::GetRightVector(TargetCameraRotation) * GetCameraBehaviorParam(TEXT("CameraOffset_Y")) +
		UKismetMathLibrary::GetUpVector(TargetCameraRotation) * GetCameraBehaviorParam(TEXT("CameraOffset_Z")),
		PivotTarget.GetLocation() + DebugViewOffset,
		GetCameraBehaviorParam(TEXT("Override_Debug")));

	// Step 6: Trace for an object between the camera and character to apply a corrective offset.
	// Trace origins are set within the Character BP via the Camera Interface.
	// Functions like the normal spring arm, but can allow for different trace origins regardless of the pivot
	FVector TraceOrigin;
	float TraceRadius = 0.0f;
	ECollisionChannel TraceChannel = ControlledCharacter->GetThirdPersonTraceParams(TraceOrigin, TraceRadius);

	UWorld* World = GetWorld();
	check(World);

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	FHitResult HitResult;
	World->SweepSingleByChannel(HitResult, TraceOrigin, TargetCameraLocation, FQuat::Identity,
	                            TraceChannel, FCollisionShape::MakeSphere(TraceRadius), Params);

	if (HitResult.IsValidBlockingHit())
	{
		TargetCameraLocation += (HitResult.Location - HitResult.TraceEnd);
	}

	// Step 7: Draw Debug Shapes.
	DrawDebugTargets(PivotTarget.GetLocation());

	// Step 8: Lerp First Person Override and return target camera parameters.
	FTransform TargetCameraTransform(TargetCameraRotation, TargetCameraLocation, FVector::OneVector);
	FTransform FPTargetCameraTransform(TargetCameraRotation, FPTarget, FVector::OneVector);

	const FTransform& MixedTransform = UKismetMathLibrary::TLerp(TargetCameraTransform, FPTargetCameraTransform,
	                                                             GetCameraBehaviorParam(TEXT("Weight_FirstPerson")));

	const FTransform& TargetTransform = UKismetMathLibrary::TLerp(MixedTransform,
	                                                              FTransform(DebugViewRotation, TargetCameraLocation, FVector::OneVector),
	                                                              GetCameraBehaviorParam(TEXT("Override_Debug")));

	Location = TargetTransform.GetLocation();
	Rotation = TargetTransform.Rotator();
	FOV = FMath::Lerp(TPFOV, FPFOV, GetCameraBehaviorParam(TEXT("Weight_FirstPerson")));

	return true;
}
