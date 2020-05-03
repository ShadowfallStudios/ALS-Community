/** Copyright (c) 2020 Doğa Can YANIKOĞLU */


#include "Character/Animation/Notify/BMNotifyStateMovementAction.h"

#include "Character/BMBaseCharacter.h"

void UBMNotifyStateMovementAction::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	ABMBaseCharacter* BaseCharacter = Cast<ABMBaseCharacter>(MeshComp->GetOwner());
	if (BaseCharacter)
	{
		BaseCharacter->SetMovementAction(MovementAction);
	}
}

void UBMNotifyStateMovementAction::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	ABMBaseCharacter* BaseCharacter = Cast<ABMBaseCharacter>(MeshComp->GetOwner());
	if (BaseCharacter && BaseCharacter->GetMovementAction() == MovementAction)
	{
		BaseCharacter->SetMovementAction(EBMMovementAction::None);
	}
}

FString UBMNotifyStateMovementAction::GetNotifyName_Implementation() const
{
	FString Name(TEXT("Movement Action: "));
	Name.Append(*MovementActionMap.Find(MovementAction));
	return Name;
}
