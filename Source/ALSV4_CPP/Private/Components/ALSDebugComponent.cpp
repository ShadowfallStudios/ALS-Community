// Project:         Advanced Locomotion System V4 on C++
// Copyright:       Copyright (C) 2021 Doğa Can Yanıkoğlu
// License:         MIT License (http://www.opensource.org/licenses/mit-license.php)
// Source Code:     https://github.com/dyanikoglu/ALSV4_CPP
// Original Author: Doğa Can Yanıkoğlu
// Contributors:    Achim Turan

#include "Components/ALSDebugComponent.h"


#include "Character/ALSBaseCharacter.h"
#include "Character/ALSPlayerCameraManager.h"
#include "Character/Animation/ALSPlayerCameraBehavior.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "Kismet/KismetMathLibrary.h"

bool UALSDebugComponent::bDebugView = false;
bool UALSDebugComponent::bShowTraces = false;
bool UALSDebugComponent::bShowDebugShapes = false;
bool UALSDebugComponent::bShowLayerColors = false;

UALSDebugComponent::UALSDebugComponent()
{
#if UE_BUILD_SHIPPING
	PrimaryComponentTick.bCanEverTick = false;
#else
	PrimaryComponentTick.bCanEverTick = true;
#endif
}

void UALSDebugComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                       FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

#if !UE_BUILD_SHIPPING
	if (!OwnerCharacter)
	{
		return;
	}

	if (bNeedsColorReset)
	{
		bNeedsColorReset = false;
		SetResetColors();
	}

	if (bShowLayerColors)
	{
		UpdateColoringSystem();
	}
	else
	{
		bNeedsColorReset = true;
	}

	if (bShowDebugShapes)
	{
		DrawDebugSpheres();

		APlayerController* Controller = Cast<APlayerController>(OwnerCharacter->GetController());
		if (Controller)
		{
			AALSPlayerCameraManager* CamManager = Cast<AALSPlayerCameraManager>(Controller->PlayerCameraManager);
			if (CamManager)
			{
				CamManager->DrawDebugTargets(OwnerCharacter->GetThirdPersonPivotTarget().GetLocation());
			}
		}
	}
#endif
}

void UALSDebugComponent::OnComponentDestroyed(bool bDestroyingHierarchy)
{
	Super::OnComponentDestroyed(bDestroyingHierarchy);

	// Keep static values false on destroy
	bDebugView = false;
	bShowTraces = false;
	bShowDebugShapes = false;
	bShowLayerColors = false;
}

void UALSDebugComponent::PreviousFocusedDebugCharacter()
{
	if (FocusedDebugCharacterIndex == INDEX_NONE)
	{ // Return here as no AALSBaseCharacter where found during call of BeginPlay.
		// Moreover, for savety set also no focused debug character.
		DebugFocusCharacter = nullptr;
		return;
	}

	FocusedDebugCharacterIndex++;
	if (FocusedDebugCharacterIndex >= AvailableDebugCharacters.Num()) {
		FocusedDebugCharacterIndex = 0;
	}

	DebugFocusCharacter = AvailableDebugCharacters[FocusedDebugCharacterIndex];
}

void UALSDebugComponent::NextFocusedDebugCharacter()
{
	if (FocusedDebugCharacterIndex == INDEX_NONE)
	{ // Return here as no AALSBaseCharacter where found during call of BeginPlay.
		// Moreover, for savety set also no focused debug character.
		DebugFocusCharacter = nullptr;
		return;
	}

	FocusedDebugCharacterIndex--;
	if (FocusedDebugCharacterIndex < 0) {
		FocusedDebugCharacterIndex = AvailableDebugCharacters.Num() - 1;
	}

	DebugFocusCharacter = AvailableDebugCharacters[FocusedDebugCharacterIndex];
}

void UALSDebugComponent::BeginPlay()
{
	Super::BeginPlay();
	
	OwnerCharacter = Cast<AALSBaseCharacter>(GetOwner());
	DebugFocusCharacter = OwnerCharacter;
	if (OwnerCharacter)
	{
		SetDynamicMaterials();
		SetResetColors();
	}

	// Get all ALSBaseCharacter's, which are currently present to show them later in the ALS HUD for debugging purposes.
	TArray<AActor*> AlsBaseCharacters;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AALSBaseCharacter::StaticClass(), AlsBaseCharacters);

	AvailableDebugCharacters.Empty();
	if (AlsBaseCharacters.Num() > 0)
	{
		AvailableDebugCharacters.Reserve(AlsBaseCharacters.Num());
		for (AActor* Character : AlsBaseCharacters)
		{
			if (AALSBaseCharacter* AlsBaseCharacter = Cast<AALSBaseCharacter>(Character))
			{
				AvailableDebugCharacters.Add(AlsBaseCharacter);
			}
		}

		FocusedDebugCharacterIndex = AvailableDebugCharacters.Find(DebugFocusCharacter);
		if (FocusedDebugCharacterIndex == INDEX_NONE && AvailableDebugCharacters.Num() > 0)
		{ // seems to be that this component was not attached to and AALSBaseCharacter,
			// therefore the index will be set to the first element in the array.
			FocusedDebugCharacterIndex = 0;
		}
	}
}

void UALSDebugComponent::UpdateColoringSystem_Implementation()
{
	FLinearColor NewColor;

	NewColor = UKismetMathLibrary::LinearColorLerp(OverlayLayerColor,AdditiveAmountColor,OwnerCharacter->GetAnimCurveValue(FName("Layering_Head_Add")));
	NewColor = UKismetMathLibrary::LinearColorLerp(BaseLayerColor,NewColor,OwnerCharacter->GetAnimCurveValue(FName("Layering_Head")));
	SetMaterialInstanceColor(Head, FName("BaseColor"), NewColor);

	NewColor = UKismetMathLibrary::LinearColorLerp(OverlayLayerColor,AdditiveAmountColor,OwnerCharacter->GetAnimCurveValue(FName("Layering_Spine_Add")));
	NewColor = UKismetMathLibrary::LinearColorLerp(BaseLayerColor,NewColor,OwnerCharacter->GetAnimCurveValue(FName("Layering_Spine")));
	SetMaterialInstanceColor(Torso, FName("BaseColor"), NewColor);

	NewColor = UKismetMathLibrary::LinearColorLerp(BaseLayerColor,AdditiveAmountColor,OwnerCharacter->GetAnimCurveValue(FName("Layering_Pelvis")));
	SetMaterialInstanceColor(Pelvis, FName("BaseColor"), NewColor);

	NewColor = UKismetMathLibrary::LinearColorLerp(BaseLayerColor,AdditiveAmountColor,OwnerCharacter->GetAnimCurveValue(FName("Layering_Legs")));
	SetMaterialInstanceColor(UpperLegs, FName("BaseColor"), NewColor);
	SetMaterialInstanceColor(LowerLegs, FName("BaseColor"), NewColor);
	SetMaterialInstanceColor(Feet, FName("BaseColor"), NewColor);

	NewColor = UKismetMathLibrary::LinearColorLerp(OverlayLayerColor,AdditiveAmountColor,OwnerCharacter->GetAnimCurveValue(FName("Layering_Arm_L_Add")));
	NewColor = UKismetMathLibrary::LinearColorLerp(BaseLayerColor,NewColor,OwnerCharacter->GetAnimCurveValue(FName("Layering_Arm_L")));
	SetMaterialInstanceColor(Shoulder_L, FName("BaseColor"), NewColor);
	SetMaterialInstanceColor(UpperArm_L, FName("BaseColor"), NewColor);
	SetMaterialInstanceColor(LowerArm_L, FName("BaseColor"), NewColor);

	NewColor = UKismetMathLibrary::LinearColorLerp(NewColor,FLinearColor::Black,OwnerCharacter->GetAnimCurveValue(FName("Layering_Hand_L")));
	NewColor = UKismetMathLibrary::LinearColorLerp(NewColor,HandIKColor,OwnerCharacter->GetAnimCurveValue(FName("Enable_HandIK_L")));
	SetMaterialInstanceColor(Hand_L, FName("BaseColor"), NewColor);

	NewColor = UKismetMathLibrary::LinearColorLerp(OverlayLayerColor,AdditiveAmountColor,OwnerCharacter->GetAnimCurveValue(FName("Layering_Arm_R_Add")));
	NewColor = UKismetMathLibrary::LinearColorLerp(BaseLayerColor,NewColor,OwnerCharacter->GetAnimCurveValue(FName("Layering_Arm_R")));
	SetMaterialInstanceColor(Shoulder_R, FName("BaseColor"), NewColor);
	SetMaterialInstanceColor(UpperArm_R, FName("BaseColor"), NewColor);
	SetMaterialInstanceColor(LowerArm_R, FName("BaseColor"), NewColor);

	NewColor = UKismetMathLibrary::LinearColorLerp(NewColor,FLinearColor::Black,OwnerCharacter->GetAnimCurveValue(FName("Layering_Hand_R")));
	NewColor = UKismetMathLibrary::LinearColorLerp(NewColor,HandIKColor,OwnerCharacter->GetAnimCurveValue(FName("Enable_HandIK_R")));
	SetMaterialInstanceColor(Hand_R, FName("BaseColor"), NewColor);
}

void UALSDebugComponent::SetResetColors_Implementation()
{
	if (bSolidColor)
	{
		SetMaterialInstanceColor(Head, FName("BaseColor"), DefaultColor);
		SetMaterialInstanceColor(Torso, FName("BaseColor"), DefaultColor);
		SetMaterialInstanceColor(Shoulder_L, FName("BaseColor"), DefaultColor);
		SetMaterialInstanceColor(UpperArm_L, FName("BaseColor"), DefaultColor);
		SetMaterialInstanceColor(LowerArm_L, FName("BaseColor"), DefaultColor);
		SetMaterialInstanceColor(Hand_L, FName("BaseColor"), DefaultColor);
		SetMaterialInstanceColor(Shoulder_R, FName("BaseColor"), DefaultColor);
		SetMaterialInstanceColor(UpperArm_R, FName("BaseColor"), DefaultColor);
		SetMaterialInstanceColor(LowerArm_R, FName("BaseColor"), DefaultColor);
		SetMaterialInstanceColor(Hand_R, FName("BaseColor"), DefaultColor);
		SetMaterialInstanceColor(Pelvis, FName("BaseColor"), DefaultColor);
		SetMaterialInstanceColor(UpperLegs, FName("BaseColor"), DefaultColor);
		SetMaterialInstanceColor(LowerLegs, FName("BaseColor"), DefaultColor);
		SetMaterialInstanceColor(Feet, FName("BaseColor"), DefaultColor);
		return;
	}

	SetMaterialInstanceColor(Head, FName("BaseColor"), SkinColor);

	switch (ShirtType)
	{
		case 0:
			SetMaterialInstanceColor(Torso, FName("BaseColor"), SkinColor);
			SetMaterialInstanceColor(Shoulder_L, FName("BaseColor"), SkinColor);
			SetMaterialInstanceColor(UpperArm_L, FName("BaseColor"), SkinColor);
			SetMaterialInstanceColor(LowerArm_L, FName("BaseColor"), SkinColor);
			SetMaterialInstanceColor(Shoulder_R, FName("BaseColor"), SkinColor);
			SetMaterialInstanceColor(UpperArm_R, FName("BaseColor"), SkinColor);
			SetMaterialInstanceColor(LowerArm_R, FName("BaseColor"), SkinColor);
			break;
		case 1:
			SetMaterialInstanceColor(Torso, FName("BaseColor"), ShirtColor);
			SetMaterialInstanceColor(Shoulder_L, FName("BaseColor"), ShirtColor);
			SetMaterialInstanceColor(UpperArm_L, FName("BaseColor"), SkinColor);
			SetMaterialInstanceColor(LowerArm_L, FName("BaseColor"), SkinColor);
			SetMaterialInstanceColor(Shoulder_R, FName("BaseColor"), ShirtColor);
			SetMaterialInstanceColor(UpperArm_R, FName("BaseColor"), SkinColor);
			SetMaterialInstanceColor(LowerArm_R, FName("BaseColor"), SkinColor);
			break;
		case 2:
		default:
			SetMaterialInstanceColor(Torso, FName("BaseColor"), ShirtColor);
			SetMaterialInstanceColor(Shoulder_L, FName("BaseColor"), ShirtColor);
			SetMaterialInstanceColor(UpperArm_L, FName("BaseColor"), ShirtColor);
			SetMaterialInstanceColor(LowerArm_L, FName("BaseColor"), SkinColor);
			SetMaterialInstanceColor(Shoulder_R, FName("BaseColor"), ShirtColor);
			SetMaterialInstanceColor(UpperArm_R, FName("BaseColor"), ShirtColor);
			SetMaterialInstanceColor(LowerArm_R, FName("BaseColor"), SkinColor);
			break;
		case 3:
			SetMaterialInstanceColor(Torso, FName("BaseColor"), ShirtColor);
			SetMaterialInstanceColor(Shoulder_L, FName("BaseColor"), ShirtColor);
			SetMaterialInstanceColor(UpperArm_L, FName("BaseColor"), ShirtColor);
			SetMaterialInstanceColor(LowerArm_L, FName("BaseColor"), ShirtColor);
			SetMaterialInstanceColor(Shoulder_R, FName("BaseColor"), ShirtColor);
			SetMaterialInstanceColor(UpperArm_R, FName("BaseColor"), ShirtColor);
			SetMaterialInstanceColor(LowerArm_R, FName("BaseColor"), ShirtColor);
			break;
	}

	switch (PantsType)
	{
		case 0:
			SetMaterialInstanceColor(Pelvis, FName("BaseColor"), PantsColor);
			SetMaterialInstanceColor(UpperLegs, FName("BaseColor"), SkinColor);
			SetMaterialInstanceColor(LowerLegs, FName("BaseColor"), SkinColor);
			break;
		case 1:
			SetMaterialInstanceColor(Pelvis, FName("BaseColor"), PantsColor);
			SetMaterialInstanceColor(UpperLegs, FName("BaseColor"), PantsColor);
			SetMaterialInstanceColor(LowerLegs, FName("BaseColor"), SkinColor);
			break;
		case 2:
		default:
			SetMaterialInstanceColor(Pelvis, FName("BaseColor"), PantsColor);
			SetMaterialInstanceColor(UpperLegs, FName("BaseColor"), PantsColor);
			SetMaterialInstanceColor(LowerLegs, FName("BaseColor"), PantsColor);
			break;
	}

	if (bShoes)
	{
		SetMaterialInstanceColor(Feet, FName("BaseColor"), ShoeColor);
	}
	else
	{
		SetMaterialInstanceColor(Feet, FName("BaseColor"), SkinColor);
	}

	if (bGloves)
	{
		SetMaterialInstanceColor(Hand_L, FName("BaseColor"), GlovesColor);
		SetMaterialInstanceColor(Hand_R, FName("BaseColor"), ShoeColor);
	}
	else
	{
		SetMaterialInstanceColor(Hand_L, FName("BaseColor"), SkinColor);
		SetMaterialInstanceColor(Hand_R, FName("BaseColor"), SkinColor);
	}
}

void UALSDebugComponent::SetDynamicMaterials_Implementation()
{
	if (OwnerCharacter)
	{
		Head = OwnerCharacter->GetMesh()->CreateDynamicMaterialInstance(OwnerCharacter->GetMesh()->GetMaterialIndex(FName("Head_MAT")));
		Torso = OwnerCharacter->GetMesh()->CreateDynamicMaterialInstance(OwnerCharacter->GetMesh()->GetMaterialIndex(FName("Torso_MAT")));
		Shoulder_L = OwnerCharacter->GetMesh()->CreateDynamicMaterialInstance(OwnerCharacter->GetMesh()->GetMaterialIndex(FName("Shoulder_L_MAT")));
		UpperArm_L = OwnerCharacter->GetMesh()->CreateDynamicMaterialInstance(OwnerCharacter->GetMesh()->GetMaterialIndex(FName("UpperArm_L_MAT")));
		LowerArm_L = OwnerCharacter->GetMesh()->CreateDynamicMaterialInstance(OwnerCharacter->GetMesh()->GetMaterialIndex(FName("LowerArm_L_MAT")));
		Hand_L = OwnerCharacter->GetMesh()->CreateDynamicMaterialInstance(OwnerCharacter->GetMesh()->GetMaterialIndex(FName("Hand_L_MAT")));
		Shoulder_R = OwnerCharacter->GetMesh()->CreateDynamicMaterialInstance(OwnerCharacter->GetMesh()->GetMaterialIndex(FName("Shoulder_R_MAT")));
		UpperArm_R = OwnerCharacter->GetMesh()->CreateDynamicMaterialInstance(OwnerCharacter->GetMesh()->GetMaterialIndex(FName("UpperArm_R_MAT")));
		LowerArm_R = OwnerCharacter->GetMesh()->CreateDynamicMaterialInstance(OwnerCharacter->GetMesh()->GetMaterialIndex(FName("LowerArm_R_MAT")));
		Hand_R = OwnerCharacter->GetMesh()->CreateDynamicMaterialInstance(OwnerCharacter->GetMesh()->GetMaterialIndex(FName("Hand_R_MAT")));
		Pelvis = OwnerCharacter->GetMesh()->CreateDynamicMaterialInstance(OwnerCharacter->GetMesh()->GetMaterialIndex(FName("Pelvis_MAT")));
		UpperLegs = OwnerCharacter->GetMesh()->CreateDynamicMaterialInstance(OwnerCharacter->GetMesh()->GetMaterialIndex(FName("UpperLeg_MAT")));
		LowerLegs = OwnerCharacter->GetMesh()->CreateDynamicMaterialInstance(OwnerCharacter->GetMesh()->GetMaterialIndex(FName("LowerLeg_MAT")));
		Feet = OwnerCharacter->GetMesh()->CreateDynamicMaterialInstance(OwnerCharacter->GetMesh()->GetMaterialIndex(FName("Feet_MAT")));
	}
}

void UALSDebugComponent::ToggleGlobalTimeDilationLocal(float TimeDilation)
{
	if (UKismetSystemLibrary::IsStandalone(this))
	{
		UGameplayStatics::SetGlobalTimeDilation(this, TimeDilation);
	}
}

void UALSDebugComponent::ToggleSlomo()
{
	bSlomo = !bSlomo;
	ToggleGlobalTimeDilationLocal(bSlomo ? 0.15f : 1.f);
}

void UALSDebugComponent::ToggleDebugView()
{
	bDebugView = !bDebugView;

	AALSPlayerCameraManager* CamManager = Cast<AALSPlayerCameraManager>(
		UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0));
	if (CamManager)
	{
		UALSPlayerCameraBehavior* CameraBehavior = Cast<UALSPlayerCameraBehavior>(
			CamManager->CameraBehavior->GetAnimInstance());
		if (CameraBehavior)
		{
			CameraBehavior->bDebugView = bDebugView;
		}
	}
}

void UALSDebugComponent::ToggleDebugMesh()
{
	if (bDebugMeshVisible)
	{
		OwnerCharacter->SetVisibleMesh(DefaultSkeletalMesh);
	}
	else
	{
		DefaultSkeletalMesh = OwnerCharacter->GetMesh()->SkeletalMesh;
		OwnerCharacter->SetVisibleMesh(DebugSkeletalMesh);
	}
	bDebugMeshVisible = !bDebugMeshVisible;
}


/** Util for drawing result of single line trace  */
void UALSDebugComponent::DrawDebugLineTraceSingle(const UWorld* World,
	                                                const FVector& Start,
	                                                const FVector& End,
	                                                EDrawDebugTrace::Type
	                                                DrawDebugType,
	                                                bool bHit,
	                                                const FHitResult& OutHit,
	                                                FLinearColor TraceColor,
	                                                FLinearColor TraceHitColor,
	                                                float DrawTime)
{
	if (DrawDebugType != EDrawDebugTrace::None)
	{
		bool bPersistent = DrawDebugType == EDrawDebugTrace::Persistent;
		float LifeTime = (DrawDebugType == EDrawDebugTrace::ForDuration) ? DrawTime : 0.f;

		if (bHit && OutHit.bBlockingHit)
		{
			// Red up to the blocking hit, green thereafter
			::DrawDebugLine(World, Start, OutHit.ImpactPoint, TraceColor.ToFColor(true), bPersistent, LifeTime);
			::DrawDebugLine(World, OutHit.ImpactPoint, End, TraceHitColor.ToFColor(true), bPersistent, LifeTime);
			::DrawDebugPoint(World, OutHit.ImpactPoint, 16.0f, TraceColor.ToFColor(true), bPersistent, LifeTime);
		}
		else
		{
			// no hit means all red
			::DrawDebugLine(World, Start, End, TraceColor.ToFColor(true), bPersistent, LifeTime);
		}
	}
}

void UALSDebugComponent::DrawDebugCapsuleTraceSingle(const UWorld* World,
	                                                   const FVector& Start,
	                                                   const FVector& End,
	                                                   const FCollisionShape& CollisionShape,
	                                                   EDrawDebugTrace::Type DrawDebugType,
	                                                   bool bHit,
	                                                   const FHitResult& OutHit,
	                                                   FLinearColor TraceColor,
	                                                   FLinearColor TraceHitColor,
	                                                   float DrawTime)
{
	if (DrawDebugType != EDrawDebugTrace::None)
	{
		bool bPersistent = DrawDebugType == EDrawDebugTrace::Persistent;
		float LifeTime = (DrawDebugType == EDrawDebugTrace::ForDuration) ? DrawTime : 0.f;

		if (bHit && OutHit.bBlockingHit)
		{
			// Red up to the blocking hit, green thereafter
			::DrawDebugCapsule(World, Start, CollisionShape.GetCapsuleHalfHeight(), CollisionShape.GetCapsuleRadius(), FQuat::Identity, TraceColor.ToFColor(true), bPersistent, LifeTime);
			::DrawDebugCapsule(World, OutHit.Location, CollisionShape.GetCapsuleHalfHeight(), CollisionShape.GetCapsuleRadius(), FQuat::Identity, TraceColor.ToFColor(true), bPersistent, LifeTime);
			::DrawDebugLine(World, Start, OutHit.Location, TraceColor.ToFColor(true), bPersistent, LifeTime);
			::DrawDebugPoint(World, OutHit.ImpactPoint, 16.0f, TraceColor.ToFColor(true), bPersistent, LifeTime);

			::DrawDebugCapsule(World, End, CollisionShape.GetCapsuleHalfHeight(), CollisionShape.GetCapsuleRadius(), FQuat::Identity, TraceHitColor.ToFColor(true), bPersistent, LifeTime);
			::DrawDebugLine(World, OutHit.Location, End, TraceHitColor.ToFColor(true), bPersistent, LifeTime);
		}
		else
		{
			// no hit means all red
			::DrawDebugCapsule(World, Start, CollisionShape.GetCapsuleHalfHeight(), CollisionShape.GetCapsuleRadius(), FQuat::Identity, TraceColor.ToFColor(true), bPersistent, LifeTime);
			::DrawDebugCapsule(World, End, CollisionShape.GetCapsuleHalfHeight(), CollisionShape.GetCapsuleRadius(), FQuat::Identity, TraceColor.ToFColor(true), bPersistent, LifeTime);
			::DrawDebugLine(World, Start, End, TraceColor.ToFColor(true), bPersistent, LifeTime);
		}
	}
}

static void DrawDebugSweptSphere(const UWorld* InWorld,
	                        FVector const& Start,
	                        FVector const& End,
	                        float Radius,
	                        FColor const& Color,
	                        bool bPersistentLines = false,
	                        float LifeTime = -1.f,
	                        uint8 DepthPriority = 0)
{
	FVector const TraceVec = End - Start;
	float const Dist = TraceVec.Size();

	FVector const Center = Start + TraceVec * 0.5f;
	float const HalfHeight = (Dist * 0.5f) + Radius;

	FQuat const CapsuleRot = FRotationMatrix::MakeFromZ(TraceVec).ToQuat();
	::DrawDebugCapsule(InWorld, Center, HalfHeight, Radius, CapsuleRot, Color, bPersistentLines, LifeTime, DepthPriority);
}

void UALSDebugComponent::DrawDebugSphereTraceSingle(const UWorld* World,
	                                                  const FVector& Start,
	                                                  const FVector& End,
	                                                  const FCollisionShape& CollisionShape,
	                                                  EDrawDebugTrace::Type DrawDebugType,
	                                                  bool bHit,
	                                                  const FHitResult& OutHit,
	                                                  FLinearColor TraceColor,
	                                                  FLinearColor TraceHitColor,
	                                                  float DrawTime)
{
	if (DrawDebugType != EDrawDebugTrace::None)
	{
		bool bPersistent = DrawDebugType == EDrawDebugTrace::Persistent;
		float LifeTime = (DrawDebugType == EDrawDebugTrace::ForDuration) ? DrawTime : 0.f;

		if (bHit && OutHit.bBlockingHit)
		{
			// Red up to the blocking hit, green thereafter
			::DrawDebugSweptSphere(World, Start, OutHit.Location, CollisionShape.GetSphereRadius(), TraceColor.ToFColor(true), bPersistent, LifeTime);
			::DrawDebugSweptSphere(World, OutHit.Location, End, CollisionShape.GetSphereRadius(), TraceHitColor.ToFColor(true), bPersistent, LifeTime);
			::DrawDebugPoint(World, OutHit.ImpactPoint, 16.0f, TraceColor.ToFColor(true), bPersistent, LifeTime);
		}
		else
		{
			// no hit means all red
			::DrawDebugSweptSphere(World, Start, End, CollisionShape.GetSphereRadius(), TraceColor.ToFColor(true), bPersistent, LifeTime);
		}
	}
}

void UALSDebugComponent::SetMaterialInstanceColor(UMaterialInstanceDynamic* MaterialInstance,
	const FName ParameterName, FLinearColor Value)
{
	if (MaterialInstance != nullptr)
	{
		MaterialInstance->SetVectorParameterValue(ParameterName, Value);
	}
}