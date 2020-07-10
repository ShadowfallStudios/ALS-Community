// Copyright (C) 2020, Doga Can Yanikoglu

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "Library/ALSCharacterEnumLibrary.h"

#include "ALSAnimNotifyGroundedEntryState.generated.h"

/**
 * 
 */
UCLASS()
class ALSV4_CPP_API UALSAnimNotifyGroundedEntryState : public UAnimNotify
{
	GENERATED_BODY()

	void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

	FString GetNotifyName_Implementation() const override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AnimNotify)
	EALSGroundedEntryState GroundedEntryState = EALSGroundedEntryState::None;
};
