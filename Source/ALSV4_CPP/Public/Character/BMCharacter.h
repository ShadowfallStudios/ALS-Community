/** Copyright (c) 2020 Doğa Can YANIKOĞLU */

#pragma once

#include "CoreMinimal.h"
#include "Character/BMBaseCharacter.h"
#include "BMCharacter.generated.h"

/**
 * Specialized character class, with additional features like held object etc.
 */
UCLASS(Blueprintable, BlueprintType)
class ALSV4_CPP_API ABMCharacter : public ABMBaseCharacter
{
	GENERATED_BODY()

	ABMCharacter();

public:
	/** Implement on BP to update held objects */
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "HeldObject")
	void UpdateHeldObject();

	UFUNCTION(BlueprintCallable, Category = "HeldObject")
	void ClearHeldObject();

	UFUNCTION(BlueprintCallable, Category = "HeldObject")
	void AttachToHand(UStaticMesh* NewStaticMesh, USkeletalMesh* NewSkeletalMesh,
	                  class UClass* NewAnimClass, bool bLeftHand, FVector Offset);

	virtual void RagdollStart() override;

	virtual void RagdollEnd() override;

	virtual ECollisionChannel GetThirdPersonTraceParams(FVector& TraceOrigin, float& TraceRadius) override;

	virtual FTransform GetThirdPersonPivotTarget() override;

	virtual FVector GetFirstPersonCameraTarget() override;

protected:
	virtual void Tick(float DeltaTime) override;

	virtual void BeginPlay() override;

	virtual void OnOverlayStateChanged(EBMOverlayState PreviousState) override;

	virtual void MantleStart(
		float MantleHeight, const FBMComponentAndTransform& MantleLedgeWS, EBMMantleType MantleType) override;

	virtual void MantleEnd() override;

	/** Implement on BP to update animation states of held objects */
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "HeldObject")
	void UpdateHeldObjectAnimations();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USceneComponent* HeldObjectRoot = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USkeletalMeshComponent* SkeletalMesh = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* StaticMesh = nullptr;
};
