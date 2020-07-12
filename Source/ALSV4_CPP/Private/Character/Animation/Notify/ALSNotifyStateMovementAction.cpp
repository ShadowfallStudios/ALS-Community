// Copyright (C) 2020, Doga Can Yanikoglu


#include "Character/Animation/Notify/ALSNotifyStateMovementAction.h"

#include "Character/ALSBaseCharacter.h"

void UALSNotifyStateMovementAction::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	AALSBaseCharacter* BaseCharacter = Cast<AALSBaseCharacter>(MeshComp->GetOwner());
	if (BaseCharacter)
	{
		BaseCharacter->SetMovementAction(MovementAction);
	}
}

void UALSNotifyStateMovementAction::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	AALSBaseCharacter* BaseCharacter = Cast<AALSBaseCharacter>(MeshComp->GetOwner());
	if (BaseCharacter && BaseCharacter->GetMovementAction() == MovementAction)
	{
		BaseCharacter->SetMovementAction(EALSMovementAction::None);
	}
}

FString UALSNotifyStateMovementAction::GetNotifyName_Implementation() const
{
	FString Name(TEXT("Movement Action: "));
	Name.Append(GetEnumerationToString(MovementAction));
	return Name;
}
