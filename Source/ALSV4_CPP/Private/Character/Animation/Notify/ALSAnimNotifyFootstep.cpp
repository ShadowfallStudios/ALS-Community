// Project:         Advanced Locomotion System V4 on C++
// Copyright:       Copyright (C) 2021 Doğa Can Yanıkoğlu
// License:         MIT License (http://www.opensource.org/licenses/mit-license.php)
// Source Code:     https://github.com/dyanikoglu/ALSV4_CPP
// Original Author: Doğa Can Yanıkoğlu
// Contributors:    


#include "Character/Animation/Notify/ALSAnimNotifyFootstep.h"
#include "Math/Vector.h"
#include "Character/ALSCharacter.h"
#include "Character/ALSBaseCharacter.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Engine/World.h"

void UALSAnimNotifyFootstep::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (!MeshComp || !MeshComp->GetAnimInstance())
	{
		return;
	}

	const float MaskCurveValue = MeshComp->GetAnimInstance()->GetCurveValue(FName(TEXT("Mask_FootstepSound")));
	const float FinalVolMult = bOverrideMaskCurve ? VolumeMultiplier : VolumeMultiplier * (1.0f - MaskCurveValue);

	if (Sound)
	{
		UAudioComponent* SpawnedAudio = UGameplayStatics::SpawnSoundAttached(Sound, MeshComp, AttachPointName,
		                                                                     FVector::ZeroVector, FRotator::ZeroRotator,
		                                                                     EAttachLocation::Type::KeepRelativeOffset,
		                                                                     true, FinalVolMult, PitchMultiplier);
		UAnimInstance* CurrentAnimInstance = MeshComp->GetAnimInstance();
		APawn* PawnOwner = CurrentAnimInstance->TryGetPawnOwner();
		
		FHitResult StepHit;
		UWorld* World = GetWorld();
		TArray<AActor*> ActorsToIgnore;

		
		int32 SurfaceIndex = -1;

		if (PawnOwner == nullptr || SpawnedAudio == nullptr)
		{
			return;
		}
		AALSCharacter* Character = static_cast<AALSCharacter*>(PawnOwner);

		const FVector TraceStart = SpawnedAudio->GetComponentLocation() + FVector(0,0,50);
		const FVector TraceEnd = (TraceStart - FVector(0, 0, 100));
		
		UKismetSystemLibrary::LineTraceSingle(SpawnedAudio, TraceStart, TraceEnd, ETraceTypeQuery::TraceTypeQuery4, true, ActorsToIgnore, EDrawDebugTrace::None, StepHit, true, FLinearColor::Red, FLinearColor::Green, 5.0f);

		
		if(StepHit.bBlockingHit)
		if (StepHit.PhysMaterial != nullptr)
		{
			UPhysicalMaterial* PhysMat = StepHit.PhysMaterial.Get();

			SurfaceIndex = static_cast<int32>(PhysMat->SurfaceType);
		}
		
		


		if (SpawnedAudio)
		{
			SpawnedAudio->SetIntParameter(FName(TEXT("FootstepType")), static_cast<int32>(FootstepType));
			SpawnedAudio->SetIntParameter(FName(TEXT("SurfaceType")), SurfaceIndex);
			SpawnedAudio->SetBoolParameter(FName(TEXT("IsMale")), static_cast<bool>(Character->bIsMale));
			SpawnedAudio->SetBoolParameter(FName(TEXT("PlayStep")), static_cast<bool>(bPlayStep));
			SpawnedAudio->SetBoolParameter(FName(TEXT("PlayVoice")), static_cast<bool>(bPlayVoice));


		}
	}
}

FString UALSAnimNotifyFootstep::GetNotifyName_Implementation() const
{
	FString Name(TEXT("Footstep Type: "));
	Name.Append(GetEnumerationToString(FootstepType));
	return Name;
}
