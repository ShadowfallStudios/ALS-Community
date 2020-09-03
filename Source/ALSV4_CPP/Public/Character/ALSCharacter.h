// Project:         Advanced Locomotion System V4 on C++
// Copyright:       Copyright (C) 2020 Doğa Can Yanıkoğlu
// License:         MIT License (http://www.opensource.org/licenses/mit-license.php)
// Source Code:     https://github.com/dyanikoglu/ALSV4_CPP
// Original Author: Doğa Can Yanıkoğlu
// Contributors:    Haziq Fadhil


#pragma once

#include "CoreMinimal.h"
#include "Character/ALSBaseCharacter.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "ALSCharacter.generated.h"

/**
 * Specialized character class, with additional features like held object etc.
 */
UCLASS(Blueprintable, BlueprintType)
class ALSV4_CPP_API AALSCharacter : public AALSBaseCharacter
{
	GENERATED_BODY()

public:
	AALSCharacter(const FObjectInitializer& ObjectInitializer);

	/** Implement on BP to update held objects */
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "ALS|HeldObject")
	void UpdateHeldObject();

	UFUNCTION(BlueprintCallable, Category = "ALS|HeldObject")
	void ClearHeldObject();

	UFUNCTION(BlueprintCallable, Category = "ALS|HeldObject")
	void AttachToHand(UStaticMesh* NewStaticMesh, USkeletalMesh* NewSkeletalMesh,
	                  class UClass* NewAnimClass, bool bLeftHand, FVector Offset);

	UFUNCTION(BlueprintCallable, Category = "ALS|ColorSystem")
	void SetOrResetColors();

	UFUNCTION(BlueprintCallable, Category = "ALS|ColorSystem")
	void UpdateColoringSystem();

	UFUNCTION(BlueprintCallable, Category = "ALS|ColorSystem")
	void UpdateLayeringColors();

	UFUNCTION(BlueprintCallable, Category = "ALS|ColorSystem")
	FLinearColor GetLayerColor(bool UseOverlay, FName BaseCurveName, FName AdditiveCurveName);

	UFUNCTION(BlueprintCallable, Category = "ALS|ColorSystem")
	void SetArmsLayerColor(bool LeftArm);

	virtual void RagdollStart() override;

	virtual void RagdollEnd() override;

	virtual ECollisionChannel GetThirdPersonTraceParams(FVector& TraceOrigin, float& TraceRadius) override;

	virtual FTransform GetThirdPersonPivotTarget() override;

	virtual FVector GetFirstPersonCameraTarget() override;

protected:
	virtual void Tick(float DeltaTime) override;

	virtual void BeginPlay() override;

	virtual void OnOverlayStateChanged(EALSOverlayState PreviousState) override;

	virtual void MantleStart(
		float MantleHeight, const FALSComponentAndTransform& MantleLedgeWS, EALSMantleType MantleType) override;

	virtual void MantleEnd() override;

	/** Implement on BP to update animation states of held objects */
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "ALS|HeldObject")
	void UpdateHeldObjectAnimations();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USceneComponent* HeldObjectRoot = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USkeletalMeshComponent* SkeletalMesh = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* StaticMesh = nullptr;

protected:
	/** Colour System*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS|ColorSystem")
	bool bSolidColor = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS|ColorSystem")
	bool bShoes = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS|ColorSystem")
	bool bGloves = true;

	/** Default Colour System*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS|ColorSystem")
	FLinearColor ALSDefaultColor = FLinearColor(0.2f, 0.2f, 1.0f, 1.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS|ColorSystem")
	FLinearColor ALSSkinColor = FLinearColor(0.760525f, 0.376263f, 0.250159f, 1.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS|ColorSystem")
	FLinearColor ALSShirtColor = FLinearColor(0.5625f, 0.375f, 0.75f, 1.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS|ColorSystem")
	FLinearColor ALSPantsColor = FLinearColor(0.5f, 0.2f, 0.05f, 1.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS|ColorSystem")
	FLinearColor ALSShoesColor = FLinearColor(0.75f, 0.75f, 0.75f, 1.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS|ColorSystem")
	FLinearColor ALSGlovesColor = FLinearColor(0.026042f, 0.026042f, 0.026042f, 1.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS|ColorSystem")
	int32 ALSShirtType = 2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS|ColorSystem")
	int32 ALSPantsType = 2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RuntimeReferences")
	UMaterialInstanceDynamic* Head = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RuntimeReferences")
	UMaterialInstanceDynamic* Torso = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RuntimeReferences")
	UMaterialInstanceDynamic* Pelvis = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RuntimeReferences")
	UMaterialInstanceDynamic* Shoulder_L = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RuntimeReferences")
	UMaterialInstanceDynamic* UpperArm_L = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RuntimeReferences")
	UMaterialInstanceDynamic* LowerArm_L = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RuntimeReferences")
	UMaterialInstanceDynamic* Hand_L = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RuntimeReferences")
	UMaterialInstanceDynamic* Shoulder_R = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RuntimeReferences")
	UMaterialInstanceDynamic* UpperArm_R = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RuntimeReferences")
	UMaterialInstanceDynamic* LowerArm_R = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RuntimeReferences")
	UMaterialInstanceDynamic* Hand_R = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RuntimeReferences")
	UMaterialInstanceDynamic* UpperLegs = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RuntimeReferences")
	UMaterialInstanceDynamic* LowerLegs = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RuntimeReferences")
	UMaterialInstanceDynamic* Feet = nullptr;

	/** Layering Colour System*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS|ColorSystem")
	FLinearColor BaseLayerColor = FLinearColor(0.0f, 0.0f, 0.0f, 1.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS|ColorSystem")
	FLinearColor OverlayLayerColor = FLinearColor(1.0f, 1.0f, 1.0f, 1.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS|ColorSystem")
	FLinearColor AdditiveAmount_Color = FLinearColor(1.0f, 0.0f, 0.0f, 1.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS|ColorSystem")
	FLinearColor Hand_Color = FLinearColor(1.0f, 0.666667f, 1.0f, 1.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS|ColorSystem")
	FLinearColor HandIK_Color = FLinearColor(0.0f, 1.0f, 0.0f, 1.0f);

private:
	bool bCanSetColors = true;
};