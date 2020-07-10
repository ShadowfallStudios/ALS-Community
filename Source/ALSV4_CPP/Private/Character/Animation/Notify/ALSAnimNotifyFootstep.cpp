// Copyright (C) 2020, Doga Can Yanikoglu


#include "Character/Animation/Notify/ALSAnimNotifyFootstep.h"


#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"

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
		if (SpawnedAudio)
		{
			SpawnedAudio->SetIntParameter(FName(TEXT("FootstepType")), static_cast<int32>(FootstepType));
		}
	}
}

FString UALSAnimNotifyFootstep::GetNotifyName_Implementation() const
{
	FString Name(TEXT("Footstep Type: "));
	Name.Append(GetEnumerationToString(FootstepType));
	return Name;
}
