// Project:         Advanced Locomotion System V4 on C++
// Copyright:       Copyright (C) 2021 Doğa Can Yanıkoğlu
// License:         MIT License (http://www.opensource.org/licenses/mit-license.php)
// Source Code:     https://github.com/dyanikoglu/ALSV4_CPP
// Original Author: Doğa Can Yanıkoğlu
// Contributors:    


#pragma once

#include "CoreMinimal.h"
#include "Runtime/Engine/Classes/Sound/SoundBase.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "Character/ALSBaseCharacter.h"
#include "Library/ALSCharacterEnumLibrary.h"

#include "Containers/Array.h"

#include "ALSAnimNotifyFootstep.generated.h"

/**
 * Character footstep anim notify
 */
UCLASS()
class ALSV4_CPP_API UALSAnimNotifyFootstep : public UAnimNotify
{
	GENERATED_BODY()

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

	virtual FString GetNotifyName_Implementation() const override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AnimNotify)
	USoundBase* Sound = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AnimNotify)
	FName AttachPointName = FName(TEXT("root"));

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AnimNotify)
	EALSFootstepType FootstepType = EALSFootstepType::Step;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AnimNotify)
	bool bPlayStep = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AnimNotify)
	bool bPlayVoice = (FootstepType == EALSFootstepType::Land || FootstepType == EALSFootstepType::Jump) ? true : false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AnimNotify)
	float VolumeMultiplier = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AnimNotify)
	float PitchMultiplier = 1.0f;;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AnimNotify)
	bool bOverrideMaskCurve = false;

};
