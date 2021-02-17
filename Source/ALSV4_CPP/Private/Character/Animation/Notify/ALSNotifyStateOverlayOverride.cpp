// Project:         Advanced Locomotion System V4 on C++
// Copyright:       Copyright (C) 2021 Doğa Can Yanıkoğlu
// License:         MIT License (http://www.opensource.org/licenses/mit-license.php)
// Source Code:     https://github.com/dyanikoglu/ALSV4_CPP
// Original Author: Doğa Can Yanıkoğlu
// Contributors:    


#include "Character/Animation/Notify/ALSNotifyStateOverlayOverride.h"

#include "Character/Animation/ALSCharacterAnimInstance.h"

void UALSNotifyStateOverlayOverride::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                                 float TotalDuration)
{
	UALSCharacterAnimInstance* AnimInst = Cast<UALSCharacterAnimInstance>(MeshComp->GetAnimInstance());
	if (AnimInst)
	{
		AnimInst->SetOverlayOverrideState(OverlayOverrideState);
	}
}

void UALSNotifyStateOverlayOverride::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	UALSCharacterAnimInstance* AnimInst = Cast<UALSCharacterAnimInstance>(MeshComp->GetAnimInstance());
	if (AnimInst)
	{
		AnimInst->SetOverlayOverrideState(0);
	}
}

FString UALSNotifyStateOverlayOverride::GetNotifyName_Implementation() const
{
	FString Name(TEXT("Overlay Override State: "));
	Name.Append(FString::FromInt(OverlayOverrideState));
	return Name;
}
