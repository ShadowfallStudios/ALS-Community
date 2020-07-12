// Copyright (C) 2020, Doga Can Yanikoglu


#include "Character/Animation/Notify/ALSNotifyStateEarlyBlendOut.h"

#include "Character/ALSBaseCharacter.h"

void UALSNotifyStateEarlyBlendOut::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                             float FrameDeltaTime)
{
	if (!MeshComp || !MeshComp->GetAnimInstance())
	{
		return;
	}

	UAnimInstance* AnimInstance = MeshComp->GetAnimInstance();
	AALSBaseCharacter* OwnerCharacter = Cast<AALSBaseCharacter>(MeshComp->GetOwner());
	if (!OwnerCharacter)
	{
		return;
	}

	bool bStopMontage = false;
	if (bCheckMovementState && OwnerCharacter->GetMovementState() == MovementStateEquals)
	{
		bStopMontage = true;
	}
	else if (bCheckStance && OwnerCharacter->GetStance() == StanceEquals)
	{
		bStopMontage = true;
	}
	else if (bCheckMovementInput && OwnerCharacter->HasMovementInput())
	{
		bStopMontage = true;
	}

	if (bStopMontage)
	{
		AnimInstance->Montage_Stop(BlendOutTime, ThisMontage);
	}
}

FString UALSNotifyStateEarlyBlendOut::GetNotifyName_Implementation() const
{
	return FString(TEXT("Early Blend Out"));
}
