// Copyright (C) 2020, Doga Can Yanikoglu

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "Library/ALSCharacterEnumLibrary.h"

#include "ALSAnimNotifyFootstep.generated.h"

/**
 * Character footstep anim notify
 */
UCLASS()
class ALSV4_CPP_API UALSAnimNotifyFootstep : public UAnimNotify
{
	GENERATED_BODY()

	void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

	FString GetNotifyName_Implementation() const override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AnimNotify)
	USoundBase* Sound = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AnimNotify)
	FName AttachPointName = FName(TEXT("Root"));

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AnimNotify)
	EALSFootstepType FootstepType = EALSFootstepType::Step;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AnimNotify)
	float VolumeMultiplier = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AnimNotify)
	float PitchMultiplier = 1.0f;;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AnimNotify)
	bool bOverrideMaskCurve = false;
};
