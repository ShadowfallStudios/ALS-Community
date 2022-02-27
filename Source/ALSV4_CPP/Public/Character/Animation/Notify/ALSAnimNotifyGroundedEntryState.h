// Copyright:       Copyright (C) 2022 Doğa Can Yanıkoğlu
// Source Code:     https://github.com/dyanikoglu/ALS-Community


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

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

	virtual FString GetNotifyName_Implementation() const override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AnimNotify)
	EALSGroundedEntryState GroundedEntryState = EALSGroundedEntryState::None;
};
