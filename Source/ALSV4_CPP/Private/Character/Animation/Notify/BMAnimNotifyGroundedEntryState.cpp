/** Copyright (c) 2020 Doğa Can YANIKOĞLU */


#include "Character/Animation/Notify/BMAnimNotifyGroundedEntryState.h"

#include "Character/Animation/BMCharacterAnimInstance.h"

void UBMAnimNotifyGroundedEntryState::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	UBMCharacterAnimInstance* AnimIns = Cast<UBMCharacterAnimInstance>(MeshComp->GetAnimInstance());
	if (AnimIns)
	{
		AnimIns->SetGroundedEntryState(GroundedEntryState);
	}
}

FString UBMAnimNotifyGroundedEntryState::GetNotifyName_Implementation() const
{
	FString Name(TEXT("Grounded Entry State: "));
	Name.Append(*GroundedEntryStateMap.Find(GroundedEntryState));
	return Name;
}
