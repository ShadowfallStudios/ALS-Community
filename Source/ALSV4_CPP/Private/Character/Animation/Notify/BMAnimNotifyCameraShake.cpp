// Copyright (C) 2020, Doga Can Yanikoglu


#include "Character/Animation/Notify/BMAnimNotifyCameraShake.h"


void UBMAnimNotifyCameraShake::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	APawn* OwnerPawn = Cast<APawn>(MeshComp->GetOwner());
	if (OwnerPawn)
	{
		APlayerController* OwnerController = Cast<APlayerController>(OwnerPawn->GetController());
		if (OwnerController)
		{
			OwnerController->ClientPlayCameraShake(ShakeClass, Scale);
		}
	}
}
