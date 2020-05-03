/** Copyright (c) 2020 Doğa Can YANIKOĞLU */

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "BMAnimNotifyCameraShake.generated.h"

/**
 * Generic camera shake animation notify for pawns with controller enabled
 */
UCLASS()
class ALSV4_CPP_API UBMAnimNotifyCameraShake : public UAnimNotify
{
	GENERATED_BODY()

	void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AnimNotify)
	TSubclassOf<UCameraShake> ShakeClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AnimNotify)
	float Scale = 1.0f;
};
