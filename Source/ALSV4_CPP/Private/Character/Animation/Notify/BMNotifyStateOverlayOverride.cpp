/** Copyright (c) 2020 Doğa Can YANIKOĞLU */


#include "Character/Animation/Notify/BMNotifyStateOverlayOverride.h"

#include "Character/Animation/BMCharacterAnimInstance.h"

void UBMNotifyStateOverlayOverride::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	UBMCharacterAnimInstance* AnimInst = Cast<UBMCharacterAnimInstance>(MeshComp->GetAnimInstance());
	if (AnimInst)
	{
		AnimInst->SetOverlayOverrideState(OverlayOverrideState);
	}
}

void UBMNotifyStateOverlayOverride::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	UBMCharacterAnimInstance* AnimInst = Cast<UBMCharacterAnimInstance>(MeshComp->GetAnimInstance());
	if (AnimInst)
	{
		AnimInst->SetOverlayOverrideState(0);
	}
}

FString UBMNotifyStateOverlayOverride::GetNotifyName_Implementation() const
{
	FString Name(TEXT("Overlay Override State: "));
	Name.Append(*OverlayOverrideStateMap.Find(OverlayOverrideState));
	return Name;
}
