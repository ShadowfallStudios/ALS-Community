/** Copyright (c) 2020 Doğa Can YANIKOĞLU */

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "Library/BMCharacterEnumLibrary.h"

#include "BMNotifyStateEarlyBlendOut.generated.h"

/**
 * Character early blend out anim state
 */
UCLASS()
class ALSV4_CPP_API UBMNotifyStateEarlyBlendOut : public UAnimNotifyState
{
	GENERATED_BODY()

	void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime) override;

	FString GetNotifyName_Implementation() const override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AnimNotify)
	UAnimMontage* ThisMontage = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AnimNotify)
	float BlendOutTime = 0.25f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AnimNotify)
	bool bCheckMovementState = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AnimNotify)
	EBMMovementState MovementStateEquals = EBMMovementState::None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AnimNotify)
	bool bCheckStance = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AnimNotify)
	EBMStance StanceEquals = EBMStance::Standing;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AnimNotify)
	bool bCheckMovementInput = false;
};
