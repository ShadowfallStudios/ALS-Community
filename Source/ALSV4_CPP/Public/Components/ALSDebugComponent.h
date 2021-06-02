// Project:         Advanced Locomotion System V4 on C++
// Copyright:       Copyright (C) 2021 Doğa Can Yanıkoğlu
// License:         MIT License (http://www.opensource.org/licenses/mit-license.php)
// Source Code:     https://github.com/dyanikoglu/ALSV4_CPP
// Original Author: Doğa Can Yanıkoğlu
// Contributors:    Achim Turan

#pragma once

#include "CoreMinimal.h"

#include "Kismet/KismetSystemLibrary.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Components/ActorComponent.h"
#include "ALSDebugComponent.generated.h"

class AALSBaseCharacter;

UCLASS(Blueprintable, BlueprintType)
class ALSV4_CPP_API UALSDebugComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UALSDebugComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	virtual void OnComponentDestroyed(bool bDestroyingHierarchy) override;

	/** Implement on BP to draw debug spheres */
	UFUNCTION(BlueprintImplementableEvent, Category = "ALS|Debug")
	void DrawDebugSpheres();

	/** BP implementable function to update layering colors */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "ALS|Debug")
	void UpdateColoringSystem();
	virtual void UpdateColoringSystem_Implementation();

	/** BP implementable function to set/reset layering colors */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "ALS|Debug")
	void SetResetColors();
	virtual void SetResetColors_Implementation();

	/** BP implementable function to set dynamic color materials for debugging */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "ALS|Debug")
	void SetDynamicMaterials();
	virtual void SetDynamicMaterials_Implementation();

	UFUNCTION(BlueprintCallable, Category = "ALS|Debug")
	void ToggleGlobalTimeDilationLocal(float TimeDilation);

	UFUNCTION(BlueprintCallable, Category = "ALS|Debug")
	void ToggleSlomo();

	UFUNCTION(BlueprintCallable, Category = "ALS|Debug")
	void ToggleHud() { bShowHud = !bShowHud; }

	UFUNCTION(BlueprintCallable, Category = "ALS|Debug")
	void ToggleDebugView();

	UFUNCTION(BlueprintCallable, Category = "ALS|Debug")
	void ToggleDebugMesh();

	UFUNCTION(BlueprintCallable, Category = "ALS|Debug")
	void ToggleTraces() { bShowTraces = !bShowTraces; }

	UFUNCTION(BlueprintCallable, Category = "ALS|Debug")
	void ToggleDebugShapes() { bShowDebugShapes = !bShowDebugShapes; }

	UFUNCTION(BlueprintCallable, Category = "ALS|Debug")
	void ToggleLayerColors() { bShowLayerColors = !bShowLayerColors; }

	UFUNCTION(BlueprintCallable, Category = "ALS|Debug")
	void ToggleCharacterInfo() { bShowCharacterInfo = !bShowCharacterInfo; }

	UFUNCTION(BlueprintCallable, Category = "ALS|Debug")
	bool GetDebugView() { return bDebugView; }

	UFUNCTION(BlueprintCallable, Category = "ALS|Debug")
	bool GetShowTraces() { return bShowTraces; }

	UFUNCTION(BlueprintCallable, Category = "ALS|Debug")
	bool GetShowDebugShapes() { return bShowDebugShapes; }

	UFUNCTION(BlueprintCallable, Category = "ALS|Debug")
	bool GetShowLayerColors() { return bShowLayerColors; }

	UFUNCTION(BlueprintCallable, Category = "ALS|Debug")
	void PreviousFocusedDebugCharacter();

	UFUNCTION(BlueprintCallable, Category = "ALS|Debug")
	void NextFocusedDebugCharacter();

	// utility functions to draw trace debug shapes,
	// which are derived from Engine/Private/KismetTraceUtils.h.
	// Sadly the functions are private, which was the reason
	// why there reimplemented here.
	static void DrawDebugLineTraceSingle(const UWorld* World,
	                                     const FVector& Start,
	                                     const FVector& End,
	                                     EDrawDebugTrace::Type DrawDebugType,
	                                     bool bHit,
	                                     const FHitResult& OutHit,
	                                     FLinearColor TraceColor,
	                                     FLinearColor TraceHitColor,
	                                     float DrawTime);

	static void DrawDebugCapsuleTraceSingle(const UWorld* World,
	                                        const FVector& Start,
	                                        const FVector& End,
	                                        const FCollisionShape& CollisionShape,
	                                        EDrawDebugTrace::Type DrawDebugType,
	                                        bool bHit,
	                                        const FHitResult& OutHit,
	                                        FLinearColor TraceColor,
	                                        FLinearColor TraceHitColor,
	                                        float DrawTime);

	static void DrawDebugSphereTraceSingle(const UWorld* World,
	                                       const FVector& Start,
	                                       const FVector& End,
	                                       const FCollisionShape& CollisionShape,
	                                       EDrawDebugTrace::Type DrawDebugType,
	                                       bool bHit,
	                                       const FHitResult& OutHit,
	                                       FLinearColor TraceColor,
	                                       FLinearColor TraceHitColor,
	                                       float DrawTime);

	static void SetMaterialInstanceColor(UMaterialInstanceDynamic* MaterialInstance,
									const FName ParameterName,
									FLinearColor Value);
protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(BlueprintReadOnly, Category = "ALS|Debug")
	AALSBaseCharacter* OwnerCharacter;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ALS|Debug")
	bool bSlomo = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ALS|Debug")
	bool bShowHud = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ALS|Debug")
	bool bShowCharacterInfo = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ALS|Debug")
	USkeletalMesh* DebugSkeletalMesh = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "ALS|Debug")
	TArray<AALSBaseCharacter*> AvailableDebugCharacters;

	UPROPERTY(BlueprintReadOnly, Category = "ALS|Debug")
	AALSBaseCharacter* DebugFocusCharacter = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "ALS|Debug")
	UMaterialInstanceDynamic* Head = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "ALS|Debug")
	UMaterialInstanceDynamic* Torso = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "ALS|Debug")
	UMaterialInstanceDynamic* Shoulder_L = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "ALS|Debug")
	UMaterialInstanceDynamic* UpperArm_L = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "ALS|Debug")
	UMaterialInstanceDynamic* LowerArm_L = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "ALS|Debug")
	UMaterialInstanceDynamic* Hand_L = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "ALS|Debug")
	UMaterialInstanceDynamic* Shoulder_R = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "ALS|Debug")
	UMaterialInstanceDynamic* UpperArm_R = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "ALS|Debug")
	UMaterialInstanceDynamic* LowerArm_R = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "ALS|Debug")
	UMaterialInstanceDynamic* Hand_R = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "ALS|Debug")
	UMaterialInstanceDynamic* Pelvis = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "ALS|Debug")
	UMaterialInstanceDynamic* UpperLegs = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "ALS|Debug")
	UMaterialInstanceDynamic* LowerLegs = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "ALS|Debug")
	UMaterialInstanceDynamic* Feet = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Colors")
	FLinearColor AdditiveAmountColor = FLinearColor(1,0,0,1);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Colors")
	FLinearColor OverlayLayerColor = FLinearColor(1,1,1,1);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Colors")
	FLinearColor BaseLayerColor = FLinearColor(0,0,0,1);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Colors")
	FLinearColor HandIKColor = FLinearColor(0,1,0,1);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Colors")
	FLinearColor HandColor = FLinearColor(0,0.66,1,1);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
	bool bSolidColor = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
	bool bShoes = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
	bool bGloves = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
	int32 PantsType = 2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
	int32 ShirtType = 2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
	FLinearColor SkinColor = FLinearColor(0.760525,0.376263,0.250159,1);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
	FLinearColor DefaultColor = FLinearColor(0.259171,0.658651,1,1);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
	FLinearColor GlovesColor = FLinearColor(0.026042,0.026042,0.026042,1);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
	FLinearColor ShoeColor = FLinearColor(0.75,0.75,0.75,1);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
	FLinearColor PantsColor = FLinearColor(0.5,0.2,0.05,1);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
	FLinearColor ShirtColor = FLinearColor(0,0,0,0);
private:
	static bool bDebugView;

	static bool bShowTraces;

	static bool bShowDebugShapes;

	static bool bShowLayerColors;

	bool bNeedsColorReset = false;

	bool bDebugMeshVisible = false;

	USkeletalMesh* DefaultSkeletalMesh = nullptr;

	/// Stores the index, which is used to select the next focused debug ALSBaseCharacter.
	/// If no characters where found during BeginPlay the value should be set to INDEX_NONE.
	int32 FocusedDebugCharacterIndex = INDEX_NONE;
};

