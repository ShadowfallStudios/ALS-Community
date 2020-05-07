// Copyright (C) 2020, Doga Can Yanikoglu


#include "Character/Animation/Notify/BMAnimNotifyGroundedEntryState.h"

#include "Character/Animation/BMCharacterAnimInstance.h"

void UBMAnimNotifyGroundedEntryState::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	UBMCharacterAnimInstance* AnimIns = Cast<UBMCharacterAnimInstance>(MeshComp->GetAnimInstance());
	if (AnimIns)
	{
		AnimIns->GroundedEntryState = GroundedEntryState;
	}
}

FString UBMAnimNotifyGroundedEntryState::GetNotifyName_Implementation() const
{
	FString Name(TEXT("Grounded Entry State: "));
	Name.Append(GetEnumerationToString(GroundedEntryState));
	return Name;
}
