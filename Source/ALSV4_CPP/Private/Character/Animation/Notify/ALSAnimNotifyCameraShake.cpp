// Copyright:       Copyright (C) 2022 Doğa Can Yanıkoğlu
// Source Code:     https://github.com/dyanikoglu/ALS-Community


#include "Character/Animation/Notify/ALSAnimNotifyCameraShake.h"

#include "Components/SkeletalMeshComponent.h"

#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerController.h"

void UALSAnimNotifyCameraShake::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	APawn* OwnerPawn = Cast<APawn>(MeshComp->GetOwner());
	if (OwnerPawn)
	{
		APlayerController* OwnerController = Cast<APlayerController>(OwnerPawn->GetController());
		if (OwnerController)
		{
			OwnerController->ClientStartCameraShake(ShakeClass, Scale);
		}
	}
}
