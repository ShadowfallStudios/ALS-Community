// Copyright (C) 2020, Doga Can Yanikoglu


#include "Character/BMCharacter.h"

ABMCharacter::ABMCharacter()
{
	HeldObjectRoot = CreateDefaultSubobject<USceneComponent>(TEXT("HeldObjectRoot"));
	HeldObjectRoot->SetupAttachment(GetMesh());

	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	SkeletalMesh->SetupAttachment(HeldObjectRoot);

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(HeldObjectRoot);
}

void ABMCharacter::ClearHeldObject()
{
	StaticMesh->SetStaticMesh(nullptr);
	SkeletalMesh->SetSkeletalMesh(nullptr);
	SkeletalMesh->SetAnimInstanceClass(nullptr);
}

void ABMCharacter::AttachToHand(UStaticMesh* NewStaticMesh, USkeletalMesh* NewSkeletalMesh, UClass* NewAnimClass,
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

void ABMCharacter::RagdollStart()
{
	ClearHeldObject();
	Super::RagdollStart();
}

void ABMCharacter::RagdollEnd()
{
	Super::RagdollEnd();
	UpdateHeldObject();
}

ECollisionChannel ABMCharacter::GetThirdPersonTraceParams(FVector& TraceOrigin, float& TraceRadius)
{
	if (bRightShoulder)
	{
		TraceOrigin = GetMesh()->GetSocketLocation(TEXT("TP_CameraTrace_R"));
		TraceRadius = 15.0f;
	}
	else
	{
		TraceOrigin = GetMesh()->GetSocketLocation(TEXT("TP_CameraTrace_L"));
		TraceRadius = 15.0f;
	}

	return ECC_Camera;
}

FTransform ABMCharacter::GetThirdPersonPivotTarget()
{
	return FTransform(GetActorRotation(),
	                  (GetMesh()->GetSocketLocation(TEXT("Head")) + GetMesh()->GetSocketLocation(TEXT("Root"))) / 2.0f,
	                  FVector::OneVector);
}

FVector ABMCharacter::GetFirstPersonCameraTarget()
{
	return GetMesh()->GetSocketLocation(TEXT("FP_Camera"));
}

void ABMCharacter::OnOverlayStateChanged(EBMOverlayState PreviousState)
{
	Super::OnOverlayStateChanged(PreviousState);
	UpdateHeldObject();
}

void ABMCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UpdateHeldObjectAnimations();
}

void ABMCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void ABMCharacter::MantleStart(float MantleHeight, const FBMComponentAndTransform& MantleLedgeWS,
                               EBMMantleType MantleType)
{
	Super::MantleStart(MantleHeight, MantleLedgeWS, MantleType);
	if (MantleType != EBMMantleType::LowMantle)
	{
		// If we're not doing low mantle, clear held object
		ClearHeldObject();
	}
}

void ABMCharacter::MantleEnd()
{
	Super::MantleEnd();
	UpdateHeldObject();
}
