// Copyright (C) 2020, Doga Can Yanikoglu

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "Library/BMCharacterEnumLibrary.h"

#include "BMAnimNotifyGroundedEntryState.generated.h"

/**
 * 
 */
UCLASS()
class ALSV4_CPP_API UBMAnimNotifyGroundedEntryState : public UAnimNotify
{
	GENERATED_BODY()

	void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

	FString GetNotifyName_Implementation() const override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AnimNotify)
	EBMGroundedEntryState GroundedEntryState = EBMGroundedEntryState::None;
};
