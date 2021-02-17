// Project:         Advanced Locomotion System V4 on C++
// Copyright:       Copyright (C) 2021 Doğa Can Yanıkoğlu
// License:         MIT License (http://www.opensource.org/licenses/mit-license.php)
// Source Code:     https://github.com/dyanikoglu/ALSV4_CPP
// Original Author: Doğa Can Yanıkoğlu
// Contributors:    


#include "Character/Animation/Notify/ALSAnimNotifyGroundedEntryState.h"

#include "Character/Animation/ALSCharacterAnimInstance.h"

void UALSAnimNotifyGroundedEntryState::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	UALSCharacterAnimInstance* AnimIns = Cast<UALSCharacterAnimInstance>(MeshComp->GetAnimInstance());
	if (AnimIns)
	{
		AnimIns->SetGroundedEntryState(GroundedEntryState);
	}
}

FString UALSAnimNotifyGroundedEntryState::GetNotifyName_Implementation() const
{
	FString Name(TEXT("Grounded Entry State: "));
	Name.Append(GetEnumerationToString(GroundedEntryState));
	return Name;
}
