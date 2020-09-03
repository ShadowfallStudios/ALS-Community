// Project:         Advanced Locomotion System V4 on C++
// Copyright:       Copyright (C) 2020 Doğa Can Yanıkoğlu
// License:         MIT License (http://www.opensource.org/licenses/mit-license.php)
// Source Code:     https://github.com/dyanikoglu/ALSV4_CPP
// Original Author: Doğa Can Yanıkoğlu
// Contributors:    Haziq Fadhil


#include "Character/ALSPlayerController.h"
#include "Character/ALSCharacter.h"
#include "Engine/StaticMesh.h"
#include "Character/AI/ALSAIController.h"

AALSCharacter::AALSCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	HeldObjectRoot = CreateDefaultSubobject<USceneComponent>(TEXT("HeldObjectRoot"));
	HeldObjectRoot->SetupAttachment(GetMesh());

	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	SkeletalMesh->SetupAttachment(HeldObjectRoot);

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(HeldObjectRoot);

	AIControllerClass = AALSAIController::StaticClass();
}

void AALSCharacter::UpdateColoringSystem()
{
	if (!PlayerController)
	{
		return;
	}
	if (PlayerController->GetShowLayerColors() && GetMesh()->IsVisible())
	{
		UpdateLayeringColors();
		bCanSetColors = true;
	}
	else
	{
		if (bCanSetColors)
		{
			bCanSetColors = false;
			SetOrResetColors();
		}
	}
}

void AALSCharacter::UpdateLayeringColors()
{
	Head->SetVectorParameterValue(FName(TEXT("BaseColor")), GetLayerColor(true, FName(TEXT("Layering_Head")), 
								  FName(TEXT("Layering_Head_Add"))));
	Torso->SetVectorParameterValue(FName(TEXT("BaseColor")), GetLayerColor(true, FName(TEXT("Layering_Spine")), 
								   FName(TEXT("Layering_Spine_Add"))));
	Pelvis->SetVectorParameterValue(FName(TEXT("BaseColor")), GetLayerColor(false, 
									FName(TEXT("Layering_Pelvis")), FName(TEXT("None"))));
	
	const FLinearColor LegsColor = GetLayerColor(false, FName(TEXT("Layering_Legs")), FName(TEXT("None")));
	UpperLegs->SetVectorParameterValue(FName(TEXT("BaseColor")), LegsColor);
	LowerLegs->SetVectorParameterValue(FName(TEXT("BaseColor")), LegsColor);
	Feet->SetVectorParameterValue(FName(TEXT("BaseColor")), LegsColor);

	SetArmsLayerColor(true);
	SetArmsLayerColor(false);
}

/** If UseOverlay is true include LinearColor Lerp for OverlayColor*/
FLinearColor AALSCharacter::GetLayerColor(bool UseOverlay, FName BaseCurveName, FName AdditiveCurveName)
{
	const float BaseAlpha = GetAnimCurveValue(BaseCurveName);
	if (UseOverlay)
	{
		const float LayeringAlpha = GetAnimCurveValue(AdditiveCurveName);
		FLinearColor OverlayColor = UKismetMathLibrary::LinearColorLerp(
			OverlayLayerColor, AdditiveAmount_Color, LayeringAlpha);
		return UKismetMathLibrary::LinearColorLerp(BaseLayerColor, OverlayColor, BaseAlpha);
	}
	else
	{
		return UKismetMathLibrary::LinearColorLerp(BaseLayerColor, AdditiveAmount_Color, BaseAlpha);
	}
}

void AALSCharacter::SetArmsLayerColor(bool LeftArm)
{
	FName BaseArmCurve = LeftArm ? FName(TEXT("Layering_Arm_L")) : FName(TEXT("Layering_Arm_R"));
	FName BaseHandCurve = LeftArm ? FName(TEXT("Layering_Hand_L")) : FName(TEXT("Layering_Hand_R"));
	FName BaseHand_IK_Curve = LeftArm ? FName(TEXT("Layering_HandIK_L")) : FName(TEXT("Layering_HandIK_R"));
	FName AdditiveArmCurve = LeftArm ? FName(TEXT("Layering_Arm_L_Add")) : FName(TEXT("Layering_Arm_R_Add"));

	UMaterialInstanceDynamic* Shoulder = LeftArm ? Shoulder_L : Shoulder_R;
	UMaterialInstanceDynamic* UpperArm = LeftArm ? UpperArm_L : UpperArm_R;
	UMaterialInstanceDynamic* LowerArm = LeftArm ? LowerArm_L : LowerArm_R;
	UMaterialInstanceDynamic* Hand = LeftArm ? Hand_L : Hand_R;

	const FLinearColor ArmsColor = GetLayerColor(true, BaseArmCurve, AdditiveArmCurve);
	Shoulder->SetVectorParameterValue(FName(TEXT("BaseColor")), ArmsColor);
	UpperArm->SetVectorParameterValue(FName(TEXT("BaseColor")), ArmsColor);
	LowerArm->SetVectorParameterValue(FName(TEXT("BaseColor")), ArmsColor);

	const FLinearColor BaseHand_Color = UKismetMathLibrary::LinearColorLerp(ArmsColor, 
										 Hand_Color, GetAnimCurveValue(BaseHandCurve));
	const FLinearColor FinalHand_Color = UKismetMathLibrary::LinearColorLerp(BaseHand_Color,
										  HandIK_Color, GetAnimCurveValue(BaseHand_IK_Curve));
	Hand->SetVectorParameterValue(FName(TEXT("BaseColor")), FinalHand_Color);
}

void AALSCharacter::ClearHeldObject()
{
	StaticMesh->SetStaticMesh(nullptr);
	SkeletalMesh->SetSkeletalMesh(nullptr);
	SkeletalMesh->SetAnimInstanceClass(nullptr);
}

void AALSCharacter::AttachToHand(UStaticMesh* NewStaticMesh, USkeletalMesh* NewSkeletalMesh, UClass* NewAnimClass,
                                 bool bLeftHand, FVector Offset)
{
	ClearHeldObject();

	if (IsValid(NewStaticMesh))
	{
		StaticMesh->SetStaticMesh(NewStaticMesh);
	}
	else if (IsValid(NewSkeletalMesh))
	{
		SkeletalMesh->SetSkeletalMesh(NewSkeletalMesh);
		if (IsValid(NewAnimClass))
		{
			SkeletalMesh->SetAnimInstanceClass(NewAnimClass);
		}
	}

	FName AttachBone;
	if (bLeftHand)
	{
		AttachBone = TEXT("VB LHS_ik_hand_gun");
	}
	else
	{
		AttachBone = TEXT("VB RHS_ik_hand_gun");
	}

	HeldObjectRoot->AttachToComponent(GetMesh(),
	                                  FAttachmentTransformRules::SnapToTargetNotIncludingScale, AttachBone);
	HeldObjectRoot->SetRelativeLocation(Offset);
}

void AALSCharacter::SetOrResetColors()
{
	FLinearColor TorsoColor;
	FLinearColor UpperArmsColor;
	FLinearColor LowerArmsColor; 
	FLinearColor PelvisColor;
	FLinearColor UpperLegsColor;
	FLinearColor LowerLegsColor;
	if (bSolidColor)
	{
		TorsoColor = ALSDefaultColor;
		UpperArmsColor = ALSDefaultColor;
		LowerArmsColor = ALSDefaultColor;
		PelvisColor = ALSDefaultColor;
		UpperLegsColor = ALSDefaultColor;
		LowerLegsColor = ALSDefaultColor;
		Head->SetVectorParameterValue(FName(TEXT("BaseColor")), ALSDefaultColor);
		Hand_L->SetVectorParameterValue(FName(TEXT("BaseColor")), ALSDefaultColor);
		Hand_R->SetVectorParameterValue(FName(TEXT("BaseColor")), ALSDefaultColor);
		Feet->SetVectorParameterValue(FName(TEXT("BaseColor")), ALSDefaultColor);
	}
	else
	{
		Head->SetVectorParameterValue(FName(TEXT("BaseColor")), ALSSkinColor);
		switch (ALSShirtType)
		{
		case 0:
			TorsoColor = ALSSkinColor;
			UpperArmsColor = ALSSkinColor;
			LowerArmsColor = ALSSkinColor;
			break;

		case 1:
			TorsoColor = ALSShirtColor;
			UpperArmsColor = ALSSkinColor;
			LowerArmsColor = ALSSkinColor;
			break;

		case 2:
			TorsoColor = ALSShirtColor;
			UpperArmsColor = ALSShirtColor;
			LowerArmsColor = ALSSkinColor;
			break;

		case 3:
			TorsoColor = ALSShirtColor;
			UpperArmsColor = ALSShirtColor;
			LowerArmsColor = ALSShirtColor;
			break;

		default:
			break;
		}

		switch (ALSPantsType)
		{
		case 0:
			PelvisColor = ALSPantsColor;
			UpperLegsColor = ALSSkinColor;
			LowerLegsColor = ALSSkinColor;
			break;

		case 1:
			PelvisColor = ALSPantsColor;
			UpperLegsColor = ALSPantsColor;
			LowerLegsColor = ALSSkinColor;
			break;

		case 2:
			PelvisColor = ALSPantsColor;
			UpperLegsColor = ALSPantsColor;
			LowerLegsColor = ALSPantsColor;
			break;

		default:
			break;
		}

		Feet->SetVectorParameterValue(FName(TEXT("BaseColor")), bShoes ? ALSShoesColor : ALSSkinColor);
		Hand_L->SetVectorParameterValue(FName(TEXT("BaseColor")), bGloves ? ALSGlovesColor : ALSSkinColor);
		Hand_R->SetVectorParameterValue(FName(TEXT("BaseColor")), bGloves ? ALSGlovesColor : ALSSkinColor);
	}

	Torso->SetVectorParameterValue(FName(TEXT("BaseColor")), TorsoColor);
	Shoulder_L->SetVectorParameterValue(FName(TEXT("BaseColor")), TorsoColor);
	Shoulder_R->SetVectorParameterValue(FName(TEXT("BaseColor")), TorsoColor);
	UpperArm_L->SetVectorParameterValue(FName(TEXT("BaseColor")), UpperArmsColor);
	UpperArm_R->SetVectorParameterValue(FName(TEXT("BaseColor")), UpperArmsColor);
	LowerArm_L->SetVectorParameterValue(FName(TEXT("BaseColor")), LowerArmsColor);
	LowerArm_R->SetVectorParameterValue(FName(TEXT("BaseColor")), LowerArmsColor);
	Pelvis->SetVectorParameterValue(FName(TEXT("BaseColor")), PelvisColor);
	UpperLegs->SetVectorParameterValue(FName(TEXT("BaseColor")), UpperLegsColor);
	LowerLegs->SetVectorParameterValue(FName(TEXT("BaseColor")), LowerLegsColor);
}

void AALSCharacter::RagdollStart()
{
	ClearHeldObject();
	Super::RagdollStart();
}

void AALSCharacter::RagdollEnd()
{
	Super::RagdollEnd();
	UpdateHeldObject();
	UpdateColoringSystem();
}

ECollisionChannel AALSCharacter::GetThirdPersonTraceParams(FVector& TraceOrigin, float& TraceRadius)
{
	FName CameraSocketName = bRightShoulder ? TEXT("TP_CameraTrace_R") : TEXT("TP_CameraTrace_L");
	TraceOrigin = GetMesh()->GetSocketLocation(CameraSocketName);
	float Pelvis_Z_Offset = GetMesh()->GetSocketTransform(TEXT("pelvis")).GetLocation().Z - 96;
	TraceOrigin.Z += Pelvis_Z_Offset;
	TraceRadius = 15.0f;
	return ECC_Camera;
}

FTransform AALSCharacter::GetThirdPersonPivotTarget()
{
	return FTransform(GetActorRotation(),
	                  (GetMesh()->GetSocketLocation(TEXT("Head")) + GetMesh()->GetSocketLocation(TEXT("Root"))) / 2.0f,
	                  FVector::OneVector);
}

FVector AALSCharacter::GetFirstPersonCameraTarget()
{
	return GetMesh()->GetSocketLocation(TEXT("FP_Camera"));
}

void AALSCharacter::OnOverlayStateChanged(EALSOverlayState PreviousState)
{
	Super::OnOverlayStateChanged(PreviousState);
	UpdateHeldObject();
}

void AALSCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UpdateHeldObjectAnimations();
	UpdateColoringSystem();
}

void AALSCharacter::BeginPlay()
{
	Super::BeginPlay();

	UpdateHeldObject();
}

void AALSCharacter::MantleStart(float MantleHeight, const FALSComponentAndTransform& MantleLedgeWS,
                                EALSMantleType MantleType)
{
	Super::MantleStart(MantleHeight, MantleLedgeWS, MantleType);
	if (MantleType != EALSMantleType::LowMantle)
	{
		// If we're not doing low mantle, clear held object
		ClearHeldObject();
	}
}

void AALSCharacter::MantleEnd()
{
	Super::MantleEnd();
	UpdateHeldObject();
}
