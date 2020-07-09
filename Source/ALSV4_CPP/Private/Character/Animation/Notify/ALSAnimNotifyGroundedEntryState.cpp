// Copyright (C) 2020, Doga Can Yanikoglu


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
